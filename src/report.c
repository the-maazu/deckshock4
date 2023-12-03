#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "headers/report.h"
#include "headers/report_items.h"
#include <stdlib.h>
#include <time.h>

static void trans_dpad(char *ds4_report, const char *sdc_report)
{
    uint8_t left = sdc_report[sdc_dpad_left.bytofst] & (1 << sdc_dpad_left.bitofst);
    uint8_t right = sdc_report[sdc_dpad_right.bytofst] & (1 << sdc_dpad_right.bitofst);
    uint8_t up = sdc_report[sdc_dpad_up.bytofst] & (1 << sdc_dpad_up.bitofst);
    uint8_t down = sdc_report[sdc_dpad_down.bytofst] & (1 << sdc_dpad_down.bitofst);

    if(left)
    {
        if(up)
            ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_NW;
        else if(down)
            ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_SW;
        else
            ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_W;
    } 
    else if(right)
    {
        if(up)
            ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_NE;
        else if(down)
            ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_SE;
        else
            ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_E;        
    }
    else if (up)
        ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_N;
    else if (down)
        ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_S;
    else 
        ds4_report[ds4_dpad.bytofst] |= DS4_DPAD_O;
}

static void trans_bools(char *ds4_report, const char *sdc_report)
{
    #define BOOL_COUNT 15
    static const boolitm *map[BOOL_COUNT][2] = {
        // face buttons
        {&sdc_btn_A, &ds4_btn_X},
        {&sdc_btn_B, &ds4_btn_O},
        {&sdc_btn_X, &ds4_btn_Sq},
        {&sdc_btn_Y, &ds4_btn_Tri},
        // // shoulder
        {&sdc_L1, &ds4_L1},
        {&sdc_R1, &ds4_R1},
        {&sdc_L2_bool, &ds4_L2_bool},
        {&sdc_R2_bool, &ds4_R2_bool},
        // options, menu and guide
        {&sdc_options, &ds4_share},
        {&sdc_menu, &ds4_options},
        {&sdc_steam, &ds4_PS},
        // trackpad
        {&sdc_tpad_click_right, &ds4_tpad_click},
        {&sdc_tpad_click_left, &ds4_tpad_click},
        // sticks
        {&sdc_L3, &ds4_L3},
        {&sdc_R3, &ds4_R3},
    };

    const boolitm *ds4;
    const boolitm *sdc;
    uint8_t sdcval;
    for (int i = 0; i < BOOL_COUNT; i++)
    {
        ds4 = map[i][1];
        sdc = map[i][0];
        sdcval = sdc_report[sdc->bytofst] >> sdc->bitofst & 1;
        ds4_report[ds4->bytofst] |= sdcval << ds4->bitofst;
    }

}

static void trans_scalar(
    char *ds4_report, 
    const char *sdc_report, 
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
        (double)(*(int8_t*) &sdc_report[offset])
        : (double)(*(uint8_t*) &sdc_report[offset]);
    else if ( nbyts == 2)
        norm = s ? 
        (double)(*(int16_t*) &sdc_report[offset])
        : (double)(*(uint16_t*) &sdc_report[offset]);
    else if ( nbyts == 4)
        norm = s ? 
        (double)(*(int32_t*) &sdc_report[offset])
        : (double)(*(uint32_t*) &sdc_report[offset]);
    norm = (norm - min)/(max - min); // normalise from 0 to 1;

    nbyts = destitm->nbyts;
    s = destitm->s;
    offset = destitm->bytofst;
    max = destitm->max;
    min = destitm->min;

    norm = norm * (max - min); // expand norm on destitm range
    norm = invert ? max - norm : min + norm;
    if( nbyts == 1) *(uint8_t *) &ds4_report[offset] = s ? (int8_t) norm : (uint8_t) norm;
    else if ( nbyts == 2) *(uint16_t *) &ds4_report[offset] = s ? (int16_t) norm : (uint16_t) norm;
    else if ( nbyts == 4) *(uint32_t *) &ds4_report[offset] = s ? (int32_t) norm : (uint32_t) norm;
}

static void trans_scalars(char *ds4_report, const char *sdc_report)
{
     #define SCALAR_COUNT 9
    static const scalitm *map[SCALAR_COUNT][2] = {
        // sticks
        {&sdc_lstick_X, &ds4_lstick_X},
        {&sdc_lstick_Y, &ds4_lstick_Y},
        {&sdc_rstick_X, &ds4_rstick_X},
        {&sdc_rstick_Y, &ds4_rstick_Y},
        // gyro and axel
        // {&sdc_accel_X, &ds4_accelX},
        // {&sdc_accel_Y, &ds4_accelY},
        // {&sdc_accel_Z, &ds4_accelZ},
        {&sdc_gyroX, &ds4_gyroX},
        {&sdc_gyroY, &ds4_gyroY},
        {&sdc_gyroZ, &ds4_gyroZ},
        // triggers
        {&sdc_L2, &ds4_L2},
        {&sdc_R2, &ds4_R2},
    };
    static const uint8_t invmap[SCALAR_COUNT] = {0,1,0,1,0,0,0,0,0,0,0,0};

    for (int i = 0; i < SCALAR_COUNT; i++)
        trans_scalar(ds4_report, sdc_report, map[i][1], map[i][0], invmap[i]);
}

int report_sdc_to_ds4(char *ds4_report, const char *sdc_report)
{
    uint8_t counter = ((uint8_t) ds4_report[ds4_counter.bytofst]  + (1 << 2))  & 0xFC;
    uint16_t tymstmp = *(uint16_t*)&ds4_report[ds4_tymstmp.bytofst] + 188;

    memset(ds4_report, 0, REP_SIZE);

    ds4_report[0] = 0x01; // report id
    ds4_report[ds4_batlvl.bytofst] = UINT8_MAX; // battery
    ds4_report[ds4_counter.bytofst] |= counter;
    *(uint16_t *) &ds4_report[ds4_tymstmp.bytofst]  = tymstmp;
     
    trans_dpad(ds4_report, sdc_report);
    trans_bools(ds4_report, sdc_report);
    trans_scalars(ds4_report, sdc_report);
}