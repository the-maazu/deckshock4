#include <asm-generic/errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/inotify.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "headers/rep_items.h"
#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "headers/trans.h"
#include "headers/ds4_items.h"
#include "headers/sdc_items.h"

#define BOOL_CNT 15
#define SCALAR_CNT 6
#define SENSOR_CNT 6

static const boolitm* default_bool_map[BOOL_CNT][2] = {
    // face buttons
    {&sdcA, &ds4X},
    {&sdcB, &ds4O},
    {&sdcX, &ds4Sq},
    {&sdcY, &ds4Tri},
    // shoulder
    {&sdcL1, &ds4L1},
    {&sdcR1, &ds4R1},
    {&sdcL2bool, &ds4L2bool},
    {&sdcR2bool, &ds4R2bool},
    // options, menu and guide
    {&sdcoptns, &ds4share},
    {&sdcmenu, &ds4optns},
    {&sdcSTEAM, &ds4ps},
    // trackpad
    {&sdcrtpadclick, &ds4tpadclick},
    {&sdcltpadclick, &ds4tpadclick},
    // sticks
    {&sdcL3, &ds4L3},
    {&sdcR3, &ds4R3},
};
static const scalitm* default_scalar_map[SCALAR_CNT][2] = {
     // sticks
    {&sdclstickX, &ds4lstickX},
    {&sdclstickY, &ds4lstickY},
    {&sdcrstickX, &ds4rstickX},
    {&sdcrstickY, &ds4rstickY},
    // triggers
    {&sdcL2, &ds4L2},
    {&sdcR2, &ds4R2},
};
static const scalitm* default_sensor_map[SENSOR_CNT][2] = {
     //  accelerometer
    {&sdcaccelX, &ds4accelX},
    {&sdcaccelY, &ds4accelY},
    {&sdcaccelZ, &ds4accelZ},
    // gyro
    {&sdcgyroP, &ds4gyroX},
    {&sdcgyroR, &ds4gyroY},
    {&sdcgyroY, &ds4gyroZ},
};
static const uint8_t default_scalar_inv_map[SCALAR_CNT][2] 
    = {0,1,0,1,0,0};
static const uint8_t default_sensor_inv_map[SENSOR_CNT] 
    = {1,0,0,0,0,1};
static const uint8_t default_sensor_ofst_map[SENSOR_CNT] 
    = {0,0,0,0,0,0};

static uint8_t disable = 0;
static boolitm* bool_map[BOOL_CNT][2] = {NULL};
static scalitm* scalar_map[SCALAR_CNT][2] = {NULL};
static scalitm* sensor_map[SENSOR_CNT][2] = {NULL};
static uint8_t scalar_inv_map[SCALAR_CNT] = {0};
static uint8_t sensor_inv_map[SENSOR_CNT] = {0};
static int16_t sensor_ofst_map[SENSOR_CNT] = {0};

inline static void set_bools(char *ds4rep, const char *sdcrep)
{
    const boolitm *ds4;
    const boolitm *sdc;
    uint8_t sdcval;
    for (int i = 0; i < BOOL_CNT; i++)
    {
        ds4 = bool_map[i][1];
        sdc = bool_map[i][0];
        if(ds4 == NULL || sdc == NULL) return;
        sdcval = sdcrep[sdc->bytofst] >> sdc->bitofst & 1;
        ds4rep[ds4->bytofst] |= sdcval << ds4->bitofst;
    }
}

inline static void set_scalar(
    char *ds4rep, 
    const char *sdcrep, 
    const scalitm * ds4itm, 
    const scalitm * sdcitm,
    const uint8_t invert,
    const uint8_t ofst
){
    if(ds4itm == NULL || sdcitm == NULL) return;

    size_t nbyts = sdcitm->nbyts;
    uint8_t s = sdcitm->s;
    uint8_t offset = sdcitm->bytofst;
    int32_t max = sdcitm->max;
    int32_t min = sdcitm->min;

    float norm;
    if( nbyts == 1)
        norm = s ? 
        (float)(*(int8_t*) &sdcrep[offset])
        : (float)(*(uint8_t*) &sdcrep[offset]);
    else if ( nbyts == 2)
        norm = s ? 
        (float)(*(int16_t*) &sdcrep[offset])
        : (float)(*(uint16_t*) &sdcrep[offset]);
    else if ( nbyts == 4)
        norm = s ? 
        (float)(*(int32_t*) &sdcrep[offset])
        : (float)(*(uint32_t*) &sdcrep[offset]);
    norm = (norm - min)/(max - min); // normalise from 0 to 1;

    nbyts = ds4itm->nbyts;
    s = ds4itm->s;
    offset = ds4itm->bytofst;
    max = ds4itm->max;
    min = ds4itm->min;

    norm = norm * (max - min); // expand norm on ds4itm range
    norm = invert ? max - norm : min + norm;
    if( nbyts == 1) *(uint8_t *) &ds4rep[offset] = s ? (int8_t) norm : (uint8_t) norm;
    else if ( nbyts == 2) *(uint16_t *) &ds4rep[offset] = s ? (int16_t) norm : (uint16_t) norm;
    else if ( nbyts == 4) *(uint32_t *) &ds4rep[offset] = s ? (int32_t) norm : (uint32_t) norm;
}

inline static void set_scalars(char *ds4rep, const char *sdcrep)
{
    for (int i = 0; i < SCALAR_CNT; i++)
        set_scalar(
            ds4rep, sdcrep, 
            scalar_map[i][1], 
            scalar_map[i][0], 
            scalar_inv_map[i], 
            0
        );
}

inline static void set_sensors(char *ds4rep, const char *sdcrep)
{
    for (int i = 0; i < SENSOR_CNT; i++)
        set_scalar(
            ds4rep, sdcrep, 
            sensor_map[i][1],
            sensor_map[i][0],
            sensor_inv_map[i],
            sensor_ofst_map[i]
        );
}

inline static void set_dpad(char *ds4rep, const char *sdcrep)
{
    uint8_t left = sdcrep[sdcdpadL.bytofst] & (1 << sdcdpadL.bitofst);
    uint8_t right = sdcrep[sdcdpadR.bytofst] & (1 << sdcdpadR.bitofst);
    uint8_t up = sdcrep[sdcdpadU.bytofst] & (1 << sdcdpadU.bitofst);
    uint8_t down = sdcrep[sdcdpadD.bytofst] & (1 << sdcdpadD.bitofst);

    if(left)
    {
        if(up)
            ds4rep[ds4dpad.bytofst] |= DS4_DPAD_NW;
        else if(down)
            ds4rep[ds4dpad.bytofst] |= DS4_DPAD_SW;
        else
            ds4rep[ds4dpad.bytofst] |= DS4_DPAD_W;
    } 
    else if(right)
    {
        if(up)
            ds4rep[ds4dpad.bytofst] |= DS4_DPAD_NE;
        else if(down)
            ds4rep[ds4dpad.bytofst] |= DS4_DPAD_SE;
        else
            ds4rep[ds4dpad.bytofst] |= DS4_DPAD_E;        
    }
    else if (up)
        ds4rep[ds4dpad.bytofst] |= DS4_DPAD_N;
    else if (down)
        ds4rep[ds4dpad.bytofst] |= DS4_DPAD_S;
    else 
        ds4rep[ds4dpad.bytofst] |= DS4_DPAD_O;
}

enum TpadSide {LeftTpad, RightTpad};
static void set_tpad_f1 (
    char * ds4rep, 
    const char * sdcrep,
    const enum TpadSide tpadside,
    const boolitm * tchsrc,
    const scalitm * scalX,
    const scalitm * scalY
){
    
    static uint8_t prevtch;
    static uint8_t count;
    static uint8_t cord[3];
    uint8_t touch = (sdcrep[tchsrc->bytofst] >> tchsrc->bitofst) & 1;
    uint16_t X, Y;

    if (touch)
    {
        X = tpadside == LeftTpad ?
            (float) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024
            : (float) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024 + 1024; // offset for right tpad
        Y = 800 - (float) (*(int16_t *)&sdcrep[scalY->bytofst] - INT16_MIN) / UINT16_MAX * 800;

        ds4rep[ds4tpadf1touch.bytofst] = (count & 0x7F);
        ds4rep[ds4tpadf1touch.bytofst + 1] |= X;
        ds4rep[ds4tpadf1touch.bytofst + 2] |= X >> 8;
        ds4rep[ds4tpadf1touch.bytofst + 2] |= Y << 4 ;
        ds4rep[ds4tpadf1touch.bytofst + 3] |= Y >> 4;
        memcpy( cord, &ds4rep[ds4tpadf1touch.bytofst + 1], 3);
    } else
    {
        ds4rep[ds4tpadf1touch.bytofst] = 0x80;
        ds4rep[ds4tpadf1touch.bytofst] |= (prevtch ? ++count : count) & 0x7F;
        memcpy( &ds4rep[ds4tpadf1touch.bytofst + 1],  cord, 3);
    }
    prevtch = touch;
}

inline static void set_tpad_f2(
    char * ds4rep, 
    const char * sdcrep, 
    const enum TpadSide tpadside,
    const boolitm * tchsrc,
    const scalitm * scalX,
    const scalitm * scalY
){
    static uint8_t prevtch;
    static uint8_t count;
    static uint8_t cord[3];
    uint8_t touch = (sdcrep[tchsrc->bytofst] >> tchsrc->bitofst) & 1;
    uint16_t X, Y;

    if (touch)
    {
        X = tpadside == LeftTpad ?
            (float) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024
            : (float) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024 + 1024; // offset for right tpad
        Y = 800 - (float) (*(int16_t *)&sdcrep[scalY->bytofst] - INT16_MIN) / UINT16_MAX * 800;

        ds4rep[ds4tpadf2touch.bytofst] = (count & 0x7F);
        ds4rep[ds4tpadf2touch.bytofst + 1] |= X;
        ds4rep[ds4tpadf2touch.bytofst + 2] |= X >> 8;
        ds4rep[ds4tpadf2touch.bytofst + 2] |= Y << 4 ;
        ds4rep[ds4tpadf2touch.bytofst + 3] |= Y >> 4;
        memcpy( cord, &ds4rep[ds4tpadf2touch.bytofst + 1], 3);
    } else
    {
        ds4rep[ds4tpadf2touch.bytofst] = 0x80;
        ds4rep[ds4tpadf2touch.bytofst] |= (prevtch ? ++count : count) & 0x7F;
        memcpy( &ds4rep[ds4tpadf2touch.bytofst + 1],  cord, 3);
    }
    prevtch = touch;
}

typedef enum touch_mode {leftfrst, righfrst} touch_mode;
inline static void set_tpad(
    char *ds4rep, const char *sdcrep, 
    uint8_t tpadtime, uint8_t timeinc
){
    static uint8_t prevtouch;
    static touch_mode mode;
    uint8_t curtouch 
        = (sdcrep[sdcrtpadtouch.bytofst] >> sdcrtpadtouch.bitofst) & 1;
    curtouch 
        |= (sdcrep[sdcltpadtouch.bytofst] >> (sdcltpadtouch.bitofst - 1)) & 2;

    if( prevtouch == 0 && curtouch == 1)
        mode = righfrst;
    if ( prevtouch == 0 && curtouch == 2)
        mode = leftfrst;
    prevtouch = curtouch;

    if (mode == leftfrst )
    {
        set_tpad_f1(
            ds4rep, sdcrep, 
            LeftTpad, // left tpad as 1st finger
            &sdcltpadtouch,
            &sdcltpadX,
            &sdcltpadY
        );
        set_tpad_f2(
            ds4rep, sdcrep,
            RightTpad, // right tpad as 2nd finger
            &sdcrtpadtouch,
            &sdcrtpadX,
            &sdcrtpadY
        );
    }
    else
    {
        set_tpad_f1(
            ds4rep, sdcrep,
            RightTpad, // right tpad as 1st finger
            &sdcrtpadtouch,
            &sdcrtpadX,
            &sdcrtpadY
        );
        set_tpad_f2(
            ds4rep, sdcrep,
            LeftTpad, // left tpad as 2nd finger
            &sdcltpadtouch,
            &sdcltpadX,
            &sdcltpadY
        );
    }

    ds4rep[ds4tpadtime.bytofst] 
        = tpadtime + (curtouch ? timeinc : 0);
}

void trans_rep_sdc_to_ds4(char *ds4rep, const char *sdcrep)
{
    // elapsed time since prev report
    // 1.25ms => 188 acquired from https://psdevwiki.com/ps4/DS4-USB
    static struct timespec prevtp;
    struct timespec curtp;
    clock_gettime(CLOCK_REALTIME, &curtp);
    uint16_t timeinc = ( curtp.tv_nsec - prevtp.tv_nsec ) * 0.0001504; // (1/1.25ms * 188) in nanoseconds

    // leave last two bits for PS and tpad click
    uint8_t counter = ((uint8_t) ds4rep[ds4counter.bytofst]  + (1 << 2))  & 0xFC;
    uint16_t timestmp = *(uint16_t*)&ds4rep[ds4tymstmp.bytofst] + timeinc;
    uint8_t tpadtime = ds4rep[ds4tpadtime.bytofst];

    memset(ds4rep, 0, REP_SIZE);

    ds4rep[0] = 0x01; // report id
    ds4rep[ds4batlvl.bytofst] = UINT8_MAX; // battery
    ds4rep[ds4counter.bytofst] = counter;
    *(uint16_t *) &ds4rep[ds4tymstmp.bytofst]  = timestmp;
    ds4rep[30] = 0x1B; // no headphones/ nothing attached
    ds4rep[33] = 0x01; // 2 finger tpad touch mode
     
    set_dpad(ds4rep, sdcrep);
    set_bools(ds4rep, sdcrep);
    set_scalars(ds4rep, sdcrep);
    set_sensors(ds4rep, sdcrep);
    set_tpad(ds4rep, sdcrep, tpadtime, timeinc);

    prevtp = curtp;
}

// Configuration
#define CONFIG_FILE "config.json"
#define CONFIG_HOMEREL_PATH "/.local/share/deckshock4/"

static char * configpath = NULL;
static int inotifd;
static int inotiwatchfd;

static int mkdir_p(const char *dir) 
{
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
    return EXIT_SUCCESS;
}

enum SdcAccelEnum{ SdcAccelDsbl=-1, SdcAccelDflt, SdcAccelX, SdcAccelY, SdcAccelZ};

struct Config {
    // uint8_t disable;
    enum SdcAccelEnum accelX;
    enum SdcAccelEnum accelY;
    enum SdcAccelEnum accelZ;
    uint8_t invaccelX;
    uint8_t invaccelY;
    uint8_t invaccelZ;
    uint8_t accelX_ofst;
    uint8_t accelY_ofst;
    uint8_t accelZ_ofst;
};

static void update_button_maps(struct Config * config)
{
    if(config == NULL) return;

    switch (config->accelX) {
        case SdcAccelX:
            sensor_map[0][0] = (scalitm *) &sdcaccelX;
            break;
        case SdcAccelY:
            sensor_map[0][0] = (scalitm *)  &sdcaccelY;
            break;
        case SdcAccelZ:
            sensor_map[0][0] = (scalitm *)  &sdcaccelZ;
            break;
        case SdcAccelDflt:
            sensor_map[0][0] = (scalitm *) &sdcaccelX;
            break;
        case SdcAccelDsbl:
            sensor_map[0][0] = NULL;
            break;
    }

    switch (config->accelY) {
        case SdcAccelX:
            sensor_map[1][0] = (scalitm *) &sdcaccelX;
            break;
        case SdcAccelY:
            sensor_map[1][0] = (scalitm *)  &sdcaccelY;
            break;
        case SdcAccelZ:
            sensor_map[1][0] = (scalitm *)  &sdcaccelZ;
            break;
        case SdcAccelDflt:
            sensor_map[1][0] = (scalitm *)  &sdcaccelY;;
            break;
        case SdcAccelDsbl:
            sensor_map[1][0] = NULL;
            break;
    }

    switch (config->accelZ) {
        case SdcAccelX:
            sensor_map[2][0] = (scalitm *) &sdcaccelX;
            break;
        case SdcAccelY:
            sensor_map[2][0] = (scalitm *)  &sdcaccelY;
            break;
        case SdcAccelZ:
            sensor_map[2][0] = (scalitm *)  &sdcaccelZ;
            break;
        case SdcAccelDflt:
            sensor_map[2][0] = (scalitm *)  &sdcaccelZ;
            break;
        case SdcAccelDsbl:
            sensor_map[2][0] = NULL;
            break;
    }
}

static void parse_config(
    enum NanoJSONCError error, 
    const char *const key, 
    const char *const value, 
    const char *const parentKey, 
    void *object
) {
    if (error != NO_ERROR) 
    { 
        fprintf(stderr, "%s\n", nanojsonc_error_desc(error)); 
        return;
    }

    struct Config **config = object;

    if (*config == NULL) {
        *config = malloc(sizeof(struct Config));
        **config = (struct Config){
            // .disable = 0,
            .accelX = SdcAccelDflt,
            .accelY = SdcAccelDflt,
            .accelZ = SdcAccelDflt,
            .invaccelX = default_sensor_inv_map[0],
            .invaccelY = default_sensor_inv_map[1],
            .invaccelZ = default_sensor_inv_map[2],
            .accelX_ofst = default_sensor_ofst_map[0],
            .accelY_ofst = default_sensor_ofst_map[1],
            .accelZ_ofst = default_sensor_ofst_map[2],
        };
    }

    if (strcmp(parentKey, "config") == 0) {
        if (strcmp(key, "disable") == 0) disable = atoi(value);
        if (strcmp(key, "accelX") == 0) (*config)->accelX = atoi(value);
        if (strcmp(key, "accelY") == 0) (*config)->accelY = atoi(value);
        if (strcmp(key, "accelY") == 0) (*config)->accelY = atoi(value);
        if (strcmp(key, "invaccelX") == 0) (*config)->accelX = atoi(value);
        if (strcmp(key, "invaccelY") == 0) (*config)->accelY = atoi(value);
        if (strcmp(key, "invaccelZ") == 0) (*config)->accelZ = atoi(value);
    }
}

static int read_parse_config() {
    FILE *file = fopen(configpath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: %d: %s\n", errno, strerror(errno));
        return errno;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking file");
        fclose(file);
        return errno;
    }
    long len = ftell(file);
    rewind(file);

    char *buffer = (char *) malloc(len + 1);
    if (buffer == NULL) {
        fclose(file);
        perror("Error allocating memory");
        return ENOMEM;
    }

    size_t bytes_read = fread(buffer, 1, len, file);
    if (bytes_read != (size_t) len) {
        if (feof(file)) {
            perror("Error: end of file reached before reading expected bytes.");
            fclose(file);
            free(buffer);
            return EIO;
        } else if (ferror(file)) {
            perror("Error reading the file");
            fclose(file);
            free(buffer);
            return errno;
        }
    }
    buffer[len] = '\0';
    fclose(file);

    struct Config *config = NULL;
    
    nanojsonc_parse_object(
        buffer, "config", 
        &config, parse_config
    );
    if(config == NULL) fputs("Failed to parse config\n", stderr);
    else fputs("Parsed config successfully\n", stderr);
    update_button_maps(config);

    free(config);
    free(buffer);
    return 0;
}

static void set_notifier()
{
    // set notification on config change
    inotifd = inotify_init1(O_NONBLOCK);
    if(inotifd > 0){
        inotiwatchfd = inotify_add_watch(inotifd, configpath, IN_MODIFY);
            if(inotiwatchfd == -1)
                fputs("Failed to register listener on config.json", stderr);
    } else fputs("Failed to init inotify", stderr);
}

static void clear_notifier()
{
    inotify_rm_watch(inotifd, inotiwatchfd);
    close(inotifd);
}

static void apply_default_map()
{
    memcpy(bool_map, default_bool_map, sizeof(boolitm*)*BOOL_CNT*2);
    memcpy(scalar_map, default_scalar_map, sizeof(scalitm*)*SCALAR_CNT*2);
    memcpy(sensor_map, default_sensor_map, sizeof(scalitm*)*SENSOR_CNT*2);
    memcpy(scalar_inv_map, default_scalar_inv_map, SCALAR_CNT);
    memcpy(sensor_inv_map, default_sensor_inv_map, SENSOR_CNT);
}

void trans_init()
{  
    struct stat st = {0};
    char * username = NULL;
    char * configdir = NULL;
    int configfd;
    
    apply_default_map();

    username = getlogin(); // username statically allocated
    if(username == NULL) 
    {
        fputs("Failed to retrieve home directory... using inbuilt default mapping\n", stderr);
        return;
    }

    size_t dir_len = strlen("/home/") 
        + strlen(CONFIG_HOMEREL_PATH) 
        + strlen(username) + 1;
    configdir = malloc(dir_len);

    strcpy(configdir, "/home/");
    strcat(configdir, username);
    strcat(configdir, "/.local/share/deckshock4/");
    
    configpath = malloc(dir_len + strlen(CONFIG_FILE)+1);
    strcpy(configpath, configdir);
    strcat(configpath, CONFIG_FILE);
    
    mkdir_p(configdir);
    free(configdir);
    configfd = open(configpath, O_CREAT );
    if(configfd == -1){
        fputs("Failed to open config file... using inbuilt default mapping\n", stderr);
        return;
    }

    read_parse_config();
    set_notifier();
    close(configfd);
    return;
}

void trans_config_probe()
{
    struct inotify_event * inotev = malloc(sizeof(struct inotify_event));
    size_t count = read(inotifd, inotev, sizeof(struct inotify_event));
    if(count == sizeof(struct inotify_event)){
        if(inotev->wd == inotiwatchfd && inotev->mask & IN_MODIFY){
            fputs("Config file modified... updating config\n", stderr);
            read_parse_config();
        }
    } else if(errno == EBADF) {
        // Bad fd after editing file with Kate
        // Setup new notifier if bad fd
        clear_notifier();
        set_notifier();
    }
    free(inotev);
}

void trans_deinit()
{
    free(configpath);
    clear_notifier();
}

uint8_t trans_is_disabled()
{
    return disable;   
}

