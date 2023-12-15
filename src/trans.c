#define _POSIX_C_SOURCE 199309L

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "headers/trans.h"
#include "headers/ds4_items.h"
#include "headers/sdc_items.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

static void set_dpad(char *ds4rep, const char *sdcrep)
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

static void set_bools(char *ds4rep, const char *sdcrep)
{
    #define BOOL_CNT 15
    static const boolitm *map[BOOL_CNT][2] = {
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

    const boolitm *ds4;
    const boolitm *sdc;
    uint8_t sdcval;
    for (int i = 0; i < BOOL_CNT; i++)
    {
        ds4 = map[i][1];
        sdc = map[i][0];
        sdcval = sdcrep[sdc->bytofst] >> sdc->bitofst & 1;
        ds4rep[ds4->bytofst] |= sdcval << ds4->bitofst;
    }

}

static void set_scalar(
    char *ds4rep, 
    const char *sdcrep, 
    const scalitm * ds4itm, 
    const scalitm * sdcitm,
    const uint8_t invert
){
    size_t nbyts = sdcitm->nbyts;
    uint8_t s = sdcitm->s;
    uint8_t offset = sdcitm->bytofst;
    int32_t max = sdcitm->max;
    int32_t min = sdcitm->min;

    double norm;
    if( nbyts == 1)
        norm = s ? 
        (double)(*(int8_t*) &sdcrep[offset])
        : (double)(*(uint8_t*) &sdcrep[offset]);
    else if ( nbyts == 2)
        norm = s ? 
        (double)(*(int16_t*) &sdcrep[offset])
        : (double)(*(uint16_t*) &sdcrep[offset]);
    else if ( nbyts == 4)
        norm = s ? 
        (double)(*(int32_t*) &sdcrep[offset])
        : (double)(*(uint32_t*) &sdcrep[offset]);
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

static void set_scalars(char *ds4rep, const char *sdcrep)
{
     #define SCALAR_CNT 12
    static const scalitm *map[SCALAR_CNT][2] = {
        // sticks
        {&sdclstickX, &ds4lstickX},
        {&sdclstickY, &ds4lstickY},
        {&sdcrstickX, &ds4rstickX},
        {&sdcrstickY, &ds4rstickY},
        // triggers
        {&sdcL2, &ds4L2},
        {&sdcR2, &ds4R2},
        //  accelerometer
        {&sdcaccelX, &ds4accelX},
        {&sdcaccelY, &ds4accelY},
        {&sdcaccelZ, &ds4accelZ},
        // gyro
        {&sdcgyroP, &ds4gyroX},
        {&sdcgyroR, &ds4gyroY},
        {&sdcgyroY, &ds4gyroZ},
    };
    static const uint8_t invmap[SCALAR_CNT] = {0,1,0,1,0,0,0,0,0,0,0,1};

    for (int i = 0; i < SCALAR_CNT; i++)
        set_scalar(ds4rep, sdcrep, map[i][1], map[i][0], invmap[i]);
}


typedef enum tpadside {lefttpad, righttpad} tpadside;
static void set_tpad_f1 (
    char * ds4rep, 
    const char * sdcrep,
    const tpadside tpadside,
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
        X = tpadside == lefttpad ?
            (double) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024
            : (double) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024 + 1024; // offset for right tpad
        Y = 800 - (double) (*(int16_t *)&sdcrep[scalY->bytofst] - INT16_MIN) / UINT16_MAX * 800;

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

static void set_tpad_f2(
    char * ds4rep, 
    const char * sdcrep, 
    const tpadside tpadside,
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
        X = tpadside == lefttpad ?
            (double) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024
            : (double) (*(int16_t *)&sdcrep[scalX->bytofst] - INT16_MIN) / UINT16_MAX * 1024 + 1024; // offset for right tpad
        Y = 800 - (double) (*(int16_t *)&sdcrep[scalY->bytofst] - INT16_MIN) / UINT16_MAX * 800;

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
static void set_tpad(char *ds4rep, const char *sdcrep, uint8_t tpadtime, uint8_t timeinc)
{
    static uint8_t prevtouch;
    static touch_mode mode;
    uint8_t curtouch = (sdcrep[sdcrtpadtouch.bytofst] >> sdcrtpadtouch.bitofst) & 1;
    curtouch |= (sdcrep[sdcltpadtouch.bytofst] >> (sdcltpadtouch.bitofst - 1)) & 2;

    if( prevtouch == 0 && curtouch == 1)
        mode = righfrst;
    if ( prevtouch == 0 && curtouch == 2)
        mode = leftfrst;
    prevtouch = curtouch;

    if (mode == leftfrst )
    {
        set_tpad_f1(ds4rep, sdcrep, lefttpad, &sdcltpadtouch ,&sdcltpadX, &sdcltpadY);
        set_tpad_f2(ds4rep, sdcrep, righttpad, &sdcrtpadtouch ,&sdcrtpadX, &sdcrtpadY);
    }
    else
    {
        set_tpad_f1(ds4rep, sdcrep, righttpad, &sdcrtpadtouch ,&sdcrtpadX, &sdcrtpadY);
        set_tpad_f2(ds4rep, sdcrep, lefttpad, &sdcltpadtouch ,&sdcltpadX, &sdcltpadY);
    }

    ds4rep[ds4tpadtime.bytofst] = tpadtime + (curtouch ? timeinc : 0);
}

int trans_rep_sdc_to_ds4(char *ds4rep, const char *sdcrep)
{
    // elapsed time since prev report
    // 1.25ms => 188 acquired from https://psdevwiki.com/ps4/DS4-USB
    static struct timespec prevtp;
    struct timespec curtp;
    clock_gettime(CLOCK_REALTIME, &curtp);
    uint8_t timeinc = ( curtp.tv_nsec - prevtp.tv_nsec ) * 0.0001504; // (1/1.25ms * 188) in nanoseconds

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
    set_tpad(ds4rep, sdcrep, tpadtime, timeinc);
}