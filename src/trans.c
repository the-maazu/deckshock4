#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/inotify.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "parser.h"

#include "headers/trans.h"
#include "headers/items.h"

#define SDC_COL 0
#define DS4_COL 1
#define BOOL_CNT 15
#define SCALAR_CNT 6
#define SENSOR_CNT 6
#define BACKBTN_CNT 4

// Enum non-negative ints map to default_bool_map row indices
enum BackButtonMap {
    BackButtonDisable=-3,
    BackButtonDefault,
    BackButtonShake,
    // Back buttoon can map to bools
    X=0,O,Sq,Tri,L1,R1,
    L2Bool, R2Bool,
    Share,Optns,PS,
    Tpad1,Tpad2,
    L3,R3
};
enum BackButton {R4=0,R5,L4,L5};
static const bool_item* back_buttons[BACKBTN_CNT] = {&sdcR4, &sdcR5,&sdcL4,&sdcL5 };
static const bv_item default_back_button_map[BACKBTN_CNT] 
    = {VIRT_ITEM(VShake), BOOL_ITEM(NULL),BOOL_ITEM(NULL),BOOL_ITEM(NULL)};

static const bool_item* default_bool_map[BOOL_CNT][2] = {
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

static const scalar_item* default_scalar_map[SCALAR_CNT][2] = {
     // sticks
    {&sdclstickX, &ds4lstickX},
    {&sdclstickY, &ds4lstickY},
    {&sdcrstickX, &ds4rstickX},
    {&sdcrstickY, &ds4rstickY},
    // triggers
    {&sdcL2, &ds4L2},
    {&sdcR2, &ds4R2},
};

// Enum non-negative ints map to default_sensor_map row indices
enum Sensor{ 
    SensorDisable=-2, 
    SensorDefault,
    AccelX, AccelY, AccelZ 
};
static const scalar_item* default_sensor_map[SENSOR_CNT][2] = {
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
static const uint8_t default_sensor_inv_map[SENSOR_CNT] = {0};
static const int16_t default_sensor_ofst_map[SENSOR_CNT] 
    = {0,-5000,-13000,0,0,0};

static uint8_t disable = 0;
static bool_item* bool_map[BOOL_CNT][2] = {NULL};
static scalar_item* scalar_map[SCALAR_CNT][2] = {NULL};
static uint8_t scalar_inv_map[SCALAR_CNT] = {0};
static scalar_item* sensor_map[SENSOR_CNT][2] = {NULL};
static uint8_t sensor_inv_map[SENSOR_CNT] = {0};
static int16_t sensor_ofst_map[SENSOR_CNT] = {0};
static bv_item back_button_map[BACKBTN_CNT] = {0};


static void apply_default_map()
{
    memcpy(bool_map, default_bool_map, sizeof(bool_item*)*BOOL_CNT*2);
    memcpy(scalar_map, default_scalar_map, sizeof(scalar_item*)*SCALAR_CNT*2);
    memcpy(sensor_map, default_sensor_map, sizeof(scalar_item*)*SENSOR_CNT*2);
    memcpy(scalar_inv_map, default_scalar_inv_map, SCALAR_CNT);
    memcpy(sensor_inv_map, default_sensor_inv_map, SENSOR_CNT);
    memcpy(sensor_ofst_map, default_sensor_ofst_map, SENSOR_CNT*sizeof(int16_t));
    memcpy(back_button_map, default_back_button_map, sizeof(bv_item)*BACKBTN_CNT);
}

static void update_accel_map(enum Sensor idx, enum Sensor mapidx)
{
     switch (mapidx) {
        case SensorDefault:
            sensor_map[idx][DS4_COL] 
                = (scalar_item *)  default_sensor_map[idx][DS4_COL];
            break;
        case SensorDisable:
            sensor_map[idx][DS4_COL] = NULL;
            break;
        case AccelX...AccelZ:
            sensor_map[idx][DS4_COL] 
                = (scalar_item *) default_sensor_map[mapidx][DS4_COL];
            break;
        default:
            fputs("Invalid accel mapping, check config.json\n", stderr);
    }
}

static void update_back_button_map(enum BackButton idx, enum BackButtonMap mapidx)
{
    switch (mapidx) {
        case BackButtonDisable:
            back_button_map[idx] = BOOL_ITEM(NULL);
            break;
        case BackButtonShake:
            back_button_map[idx] = VIRT_ITEM(VShake);
            break;
        case BackButtonDefault:
            back_button_map[idx] = default_back_button_map[idx];
            break;
        case X...R3:
            back_button_map[idx] = BOOL_ITEM((bool_item *) default_bool_map[mapidx][DS4_COL]);
            break;
        default:
            fputs("Invalid back button mapping, check config.json\n", stderr);
    }
}


inline static void set_bools(char *ds4rep, const char *sdcrep)
{
    const bool_item *ds4itm;
    const bool_item *sdcitm;
    uint8_t sdcval;
    for (int i = 0; i < BOOL_CNT; i++)
    {
        ds4itm = bool_map[i][DS4_COL];
        sdcitm = bool_map[i][SDC_COL];
        if(ds4itm == NULL || sdcitm == NULL) return;
        sdcval = sdcrep[sdcitm->byte] >> sdcitm->bit & 1;
        ds4rep[ds4itm->byte] |= sdcval << ds4itm->bit;
    }

    // back buttons
    for (int i = R4; i <= L5; i++)
    {
        sdcitm = (bool_item *) back_buttons[i];
        if(
            back_button_map[i].type == BItem 
            && back_button_map[i].value.boolitm != NULL
        ) {
            ds4itm = back_button_map[i].value.boolitm;
            sdcval = sdcrep[sdcitm->byte] >> sdcitm->bit & 1;
            ds4rep[ds4itm->byte] |= sdcval << ds4itm->bit;
        }
    }
}

inline static int shake_button_down(const char *sdcrep)
{
    
    bool_item* sdcitm;
    
    for(int i = R4; i <= L5; i++){
        sdcitm = (bool_item *) back_buttons[i];
        if(
            back_button_map[i].type == VItem 
            && back_button_map[i].value.virtitm == VShake
            && sdcrep[sdcitm->byte] >> sdcitm->bit & 1
        ) return 1;
    }
    
    return 0;
}

inline static void shake_sensors(char *ds4rep)
{
    scalar_item* ds4itm;
    float min;
    float max;
    for (int i = 0; i < SENSOR_CNT; i++){
        ds4itm = sensor_map[i][DS4_COL];
        min =  ds4itm->min;
        max = ds4itm->max;
        // pick random value between min and max
        ds4rep[ds4itm->byte] = min + (max - min) * ((double) rand()/RAND_MAX);
    }
        
}

inline static void set_scalar(
    char *ds4rep, 
    const char *sdcrep, 
    const scalar_item * ds4itm, 
    const scalar_item * sdcitm,
    const uint8_t invert,
    const int16_t offset
){
    if(ds4itm == NULL || sdcitm == NULL) return;

    size_t nbyts = sdcitm->size;
    uint8_t s = sdcitm->s;
    uint8_t bytofst = sdcitm->byte;
    int32_t max = sdcitm->max;
    int32_t min = sdcitm->min;

    float norm;
    if( nbyts == 1)
        norm = s ? 
        (float)(*(int8_t*) &sdcrep[bytofst])
        : (float)(*(uint8_t*) &sdcrep[bytofst]);
    else if ( nbyts == 2)
        norm = s ? 
        (float)(*(int16_t*) &sdcrep[bytofst])
        : (float)(*(uint16_t*) &sdcrep[bytofst]);
    else if ( nbyts == 4)
        norm = s ? 
        (float)(*(int32_t*) &sdcrep[bytofst])
        : (float)(*(uint32_t*) &sdcrep[bytofst]);
    norm += offset;
    norm = (norm - min)/(max - min); // normalise from 0 to 1;

    nbyts = ds4itm->size;
    s = ds4itm->s;
    bytofst = ds4itm->byte;
    max = ds4itm->max;
    min = ds4itm->min;

    norm = norm * (max - min); // expand norm on ds4itm range
    norm = invert ? max - norm : min + norm;
    if( nbyts == 1) *(uint8_t *) &ds4rep[bytofst] = s ? (int8_t) norm : (uint8_t) norm;
    else if ( nbyts == 2) *(uint16_t *) &ds4rep[bytofst] = s ? (int16_t) norm : (uint16_t) norm;
    else if ( nbyts == 4) *(uint32_t *) &ds4rep[bytofst] = s ? (int32_t) norm : (uint32_t) norm;
}

inline static void set_scalars(char *ds4rep, const char *sdcrep)
{
    for (int i = 0; i < SCALAR_CNT; i++)
        set_scalar(
            ds4rep, sdcrep, 
            scalar_map[i][DS4_COL], 
            scalar_map[i][SDC_COL], 
            scalar_inv_map[i], 
            0
        );
}

inline static void set_sensors(char *ds4rep, const char *sdcrep)
{
    for (int i = 0; i < SENSOR_CNT; i++)
        set_scalar(
            ds4rep, sdcrep, 
            sensor_map[i][DS4_COL],
            sensor_map[i][SDC_COL],
            sensor_inv_map[i],
            sensor_ofst_map[i]
        );
}

inline static void set_dpad(char *ds4rep, const char *sdcrep)
{
    uint8_t left = sdcrep[sdcdpadL.byte] & (1 << sdcdpadL.bit);
    uint8_t right = sdcrep[sdcdpadR.byte] & (1 << sdcdpadR.bit);
    uint8_t up = sdcrep[sdcdpadU.byte] & (1 << sdcdpadU.bit);
    uint8_t down = sdcrep[sdcdpadD.byte] & (1 << sdcdpadD.bit);

    if(left)
    {
        if(up)
            ds4rep[ds4dpad.byte] |= DS4_DPAD_NW;
        else if(down)
            ds4rep[ds4dpad.byte] |= DS4_DPAD_SW;
        else
            ds4rep[ds4dpad.byte] |= DS4_DPAD_W;
    } 
    else if(right)
    {
        if(up)
            ds4rep[ds4dpad.byte] |= DS4_DPAD_NE;
        else if(down)
            ds4rep[ds4dpad.byte] |= DS4_DPAD_SE;
        else
            ds4rep[ds4dpad.byte] |= DS4_DPAD_E;        
    }
    else if (up)
        ds4rep[ds4dpad.byte] |= DS4_DPAD_N;
    else if (down)
        ds4rep[ds4dpad.byte] |= DS4_DPAD_S;
    else 
        ds4rep[ds4dpad.byte] |= DS4_DPAD_O;
}

enum TpadSide {LeftTpad, RightTpad};
static void set_tpad_f1 (
    char * ds4rep, 
    const char * sdcrep,
    const enum TpadSide tpadside,
    const bool_item * tchsrc,
    const scalar_item * scalX,
    const scalar_item * scalY
){
    
    static uint8_t prevtch;
    static uint8_t count;
    static uint8_t cord[3];
    uint8_t touch = (sdcrep[tchsrc->byte] >> tchsrc->bit) & 1;
    uint16_t X, Y;

    if (touch)
    {
        X = tpadside == LeftTpad ?
            (float) (*(int16_t *)&sdcrep[scalX->byte] - INT16_MIN) / UINT16_MAX * 1024
            : (float) (*(int16_t *)&sdcrep[scalX->byte] - INT16_MIN) / UINT16_MAX * 1024 + 1024; // offset for right tpad
        Y = 800 - (float) (*(int16_t *)&sdcrep[scalY->byte] - INT16_MIN) / UINT16_MAX * 800;

        ds4rep[ds4tpadf1touch.byte] = (count & 0x7F);
        ds4rep[ds4tpadf1touch.byte + 1] |= X;
        ds4rep[ds4tpadf1touch.byte + 2] |= X >> 8;
        ds4rep[ds4tpadf1touch.byte + 2] |= Y << 4 ;
        ds4rep[ds4tpadf1touch.byte + 3] |= Y >> 4;
        memcpy( cord, &ds4rep[ds4tpadf1touch.byte + 1], 3);
    } else
    {
        ds4rep[ds4tpadf1touch.byte] = 0x80;
        ds4rep[ds4tpadf1touch.byte] |= (prevtch ? ++count : count) & 0x7F;
        memcpy( &ds4rep[ds4tpadf1touch.byte + 1],  cord, 3);
    }
    prevtch = touch;
}

inline static void set_tpad_f2(
    char * ds4rep, 
    const char * sdcrep, 
    const enum TpadSide tpadside,
    const bool_item * tchsrc,
    const scalar_item * scalX,
    const scalar_item * scalY
){
    static uint8_t prevtch;
    static uint8_t count;
    static uint8_t cord[3];
    uint8_t touch = (sdcrep[tchsrc->byte] >> tchsrc->bit) & 1;
    uint16_t X, Y;

    if (touch)
    {
        X = tpadside == LeftTpad ?
            (float) (*(int16_t *)&sdcrep[scalX->byte] - INT16_MIN) / UINT16_MAX * 1024
            : (float) (*(int16_t *)&sdcrep[scalX->byte] - INT16_MIN) / UINT16_MAX * 1024 + 1024; // offset for right tpad
        Y = 800 - (float) (*(int16_t *)&sdcrep[scalY->byte] - INT16_MIN) / UINT16_MAX * 800;

        ds4rep[ds4tpadf2touch.byte] = (count & 0x7F);
        ds4rep[ds4tpadf2touch.byte + 1] |= X;
        ds4rep[ds4tpadf2touch.byte + 2] |= X >> 8;
        ds4rep[ds4tpadf2touch.byte + 2] |= Y << 4 ;
        ds4rep[ds4tpadf2touch.byte + 3] |= Y >> 4;
        memcpy( cord, &ds4rep[ds4tpadf2touch.byte + 1], 3);
    } else
    {
        ds4rep[ds4tpadf2touch.byte] = 0x80;
        ds4rep[ds4tpadf2touch.byte] |= (prevtch ? ++count : count) & 0x7F;
        memcpy( &ds4rep[ds4tpadf2touch.byte + 1],  cord, 3);
    }
    prevtch = touch;
}

enum TouchMode {LeftFirst, RightFirst};
inline static void set_tpad(
    char *ds4rep, const char *sdcrep, 
    uint8_t tpadtime, uint8_t timeinc
){
    static uint8_t prevtouch;
    static enum TouchMode mode;
    uint8_t curtouch 
        = (sdcrep[sdcrtpadtouch.byte] >> sdcrtpadtouch.bit) & 1;
    curtouch 
        |= (sdcrep[sdcltpadtouch.byte] >> (sdcltpadtouch.bit - 1)) & 2;

    if( prevtouch == 0 && curtouch == 1)
        mode = RightFirst;
    if ( prevtouch == 0 && curtouch == 2)
        mode = LeftFirst;
    prevtouch = curtouch;

    if (mode == LeftFirst )
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

    ds4rep[ds4tpadtime.byte] 
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
    uint8_t counter = ((uint8_t) ds4rep[ds4counter.byte]  + (1 << 2))  & 0xFC;
    uint16_t timestmp = *(uint16_t*)&ds4rep[ds4tymstmp.byte] + timeinc;
    uint8_t tpadtime = ds4rep[ds4tpadtime.byte];

    memset(ds4rep, 0, REP_SIZE);

    ds4rep[0] = 0x01; // report id
    ds4rep[ds4batlvl.byte] = UINT8_MAX; // battery
    ds4rep[ds4counter.byte] = counter;
    *(uint16_t *) &ds4rep[ds4tymstmp.byte]  = timestmp;
    ds4rep[30] = 0x1B; // no headphones/ nothing attached
    ds4rep[33] = 0x01; // 2 finger tpad touch mode
     
    set_dpad(ds4rep, sdcrep);
    set_bools(ds4rep, sdcrep);
    set_scalars(ds4rep, sdcrep);
    if(shake_button_down(sdcrep)) shake_sensors(ds4rep);
    else set_sensors(ds4rep, sdcrep);
    set_tpad(ds4rep, sdcrep, tpadtime, timeinc);

    prevtp = curtp;
}


// CONFIG CODE BELOW
#define CONFIG_FILE "config.json"
#define CONFIG_HOMEREL_PATH "/.local/share/deckshock4/"
struct Config {
    enum Sensor accelX;
    enum Sensor accelY;
    enum Sensor accelZ;
    uint8_t invaccelX;
    uint8_t invaccelY;
    uint8_t invaccelZ;
    int16_t accelXofst;
    int16_t accelYofst;
    int16_t accelZofst;
    enum BackButtonMap L4;
    enum BackButtonMap L5;
    enum BackButtonMap R4;
    enum BackButtonMap R5;
};

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

static void update_maps(struct Config * config)
{
    if(config == NULL) return;
    apply_default_map();

    update_accel_map(AccelX, config->accelX);
    update_accel_map(AccelY, config->accelY);
    update_accel_map(AccelZ, config->accelZ);

    sensor_inv_map[AccelX] = config->invaccelX;
    sensor_inv_map[AccelY] = config->invaccelY;
    sensor_inv_map[AccelZ] = config->invaccelZ;
    sensor_ofst_map[AccelX] = config->accelXofst;
    sensor_ofst_map[AccelY] = config->accelYofst;
    sensor_ofst_map[AccelZ] = config->accelZofst;
    
    update_back_button_map(R4, config->R4);
    update_back_button_map(R5, config->R5);
    update_back_button_map(L4, config->L4);
    update_back_button_map(L5, config->L5);
    
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
            .accelX = SensorDefault,
            .accelY = SensorDefault,
            .accelZ = SensorDefault,
            .invaccelX = default_sensor_inv_map[AccelX],
            .invaccelY = default_sensor_inv_map[AccelY],
            .invaccelZ = default_sensor_inv_map[AccelX],
            .accelXofst = default_sensor_ofst_map[AccelX],
            .accelYofst = default_sensor_ofst_map[AccelY],
            .accelZofst = default_sensor_ofst_map[AccelZ],
            .L4 = BackButtonDefault,
            .L5 = BackButtonDefault,
            .R4 = BackButtonDefault,
            .R5 = BackButtonDefault,
        };
    }

    if (strcmp(parentKey, "config") == 0) {
        if (strcmp(key, "disable") == 0) disable = atoi(value);
        if (strcmp(key, "accelX") == 0) (*config)->accelX = atoi(value);
        if (strcmp(key, "accelY") == 0) (*config)->accelY = atoi(value);
        if (strcmp(key, "accelZ") == 0) (*config)->accelZ = atoi(value);
        if (strcmp(key, "invaccelX") == 0) (*config)->invaccelX = atoi(value);
        if (strcmp(key, "invaccelY") == 0) (*config)->invaccelY = atoi(value);
        if (strcmp(key, "invaccelZ") == 0) (*config)->invaccelZ = atoi(value);
        if (strcmp(key, "accelXofst") == 0) (*config)->accelXofst = atoi(value);
        if (strcmp(key, "accelYofst") == 0) (*config)->accelYofst = atoi(value);
        if (strcmp(key, "accelZofst") == 0) (*config)->accelZofst = atoi(value);
        if (strcmp(key, "L4") == 0) (*config)->L4 = atoi(value);
        if (strcmp(key, "L5") == 0) (*config)->L5 = atoi(value);
        if (strcmp(key, "R4") == 0) (*config)->R4 = atoi(value);
        if (strcmp(key, "R5") == 0) (*config)->R5 = atoi(value);
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
    update_maps(config);

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

void trans_init()
{  
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
        // Clear and setup new notifier if bad fd
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

