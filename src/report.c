#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "headers/report.h"
#include "headers/report_items.h"
#include <stdlib.h>
#include <time.h>

static void trans_dpad(char *ds4rep, const char *sdcrep)
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

static void trans_bools(char *ds4rep, const char *sdcrep)
{
    #define BOOL_COUNT 15
    static const boolitm *map[BOOL_COUNT][2] = {
        // face buttons
        {&sdcA, &ds4X},
        {&sdcB, &ds4O},
        {&sdcX, &ds4Sq},
        {&sdcY, &ds4Tri},
        // // shoulder
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
    for (int i = 0; i < BOOL_COUNT; i++)
    {
        ds4 = map[i][1];
        sdc = map[i][0];
        sdcval = sdcrep[sdc->bytofst] >> sdc->bitofst & 1;
        ds4rep[ds4->bytofst] |= sdcval << ds4->bitofst;
    }

}

static void trans_scalar(
    char *ds4rep, 
    const char *sdcrep, 
    const scalitm * destitm, 
    const scalitm * srcitm,
    const uint8_t invert
){
    size_t nbyts = srcitm->nbyts;
    uint8_t s = srcitm->s;
    uint8_t offset = srcitm->bytofst;
    int32_t max = srcitm->max;
    int32_t min = srcitm->min;

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

    nbyts = destitm->nbyts;
    s = destitm->s;
    offset = destitm->bytofst;
    max = destitm->max;
    min = destitm->min;

    norm = norm * (max - min); // expand norm on destitm range
    norm = invert ? max - norm : min + norm;
    if( nbyts == 1) *(uint8_t *) &ds4rep[offset] = s ? (int8_t) norm : (uint8_t) norm;
    else if ( nbyts == 2) *(uint16_t *) &ds4rep[offset] = s ? (int16_t) norm : (uint16_t) norm;
    else if ( nbyts == 4) *(uint32_t *) &ds4rep[offset] = s ? (int32_t) norm : (uint32_t) norm;
}

static void trans_scalars(char *ds4rep, const char *sdcrep)
{
     #define SCALAR_COUNT 9
    static const scalitm *map[SCALAR_COUNT][2] = {
        // sticks
        {&sdclstickX, &ds4lstickX},
        {&sdclstickY, &ds4lstickY},
        {&sdcrstickX, &ds4rstickX},
        {&sdcrstickY, &ds4rstickY},
        // gyro and axel
        // {&sdcaccelX, &ds4accelX},
        // {&sdcaccelY, &ds4accelY},
        // {&sdcaccelZ, &ds4accelZ},
        {&sdcgyroX, &ds4gyroX},
        {&sdcgyroY, &ds4gyroY},
        {&sdcgyroZ, &ds4gyroZ},
        // triggers
        {&sdcL2, &ds4L2},
        {&sdcR2, &ds4R2},
    };
    static const uint8_t invmap[SCALAR_COUNT] = {0,1,0,1,0,0,0,0,0,0,0,0};

    for (int i = 0; i < SCALAR_COUNT; i++)
        trans_scalar(ds4rep, sdcrep, map[i][1], map[i][0], invmap[i]);
}

int report_sdc_to_ds4(char *ds4rep, const char *sdcrep)
{
    uint8_t counter = ((uint8_t) ds4rep[ds4counter.bytofst]  + (1 << 2))  & 0xFC;
    uint16_t tymstmp = *(uint16_t*)&ds4rep[ds4tymstmp.bytofst] + 188;

    memset(ds4rep, 0, REP_SIZE);

    ds4rep[0] = 0x01; // report id
    ds4rep[ds4batlvl.bytofst] = UINT8_MAX; // battery
    ds4rep[ds4counter.bytofst] |= counter;
    *(uint16_t *) &ds4rep[ds4tymstmp.bytofst]  = tymstmp;
     
    trans_dpad(ds4rep, sdcrep);
    trans_bools(ds4rep, sdcrep);
    trans_scalars(ds4rep, sdcrep);
}