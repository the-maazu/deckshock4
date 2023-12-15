#include "rep_items.h"

// time, counter and battery
static const scalitm ds4counter = {
    .bytofst = 7,
    .nbyts = 1,
};
static const scalitm ds4batlvl = {
    .bytofst = 12,
    .nbyts = 1,
};
static const scalitm ds4tymstmp = {
    .bytofst = 10,
    .nbyts = 2,
};
//  dpad
#define DS4_DPAD_N  0
#define DS4_DPAD_NE 1
#define DS4_DPAD_E 2
#define DS4_DPAD_SE 3
#define DS4_DPAD_S 4
#define DS4_DPAD_SW 5
#define DS4_DPAD_W 6
#define DS4_DPAD_NW 7
#define DS4_DPAD_O 8
static const scalitm ds4dpad = {
    .bytofst = 5,
    .nbyts = 1,
};
// face buttons
static const boolitm ds4X = {
    .bytofst = 5,
    .bitofst = 5,
};
static const boolitm ds4O = {
    .bytofst = 5,
    .bitofst = 6,
};
static const boolitm ds4Tri = {
    .bytofst = 5,
    .bitofst = 7,
};
static const boolitm ds4Sq = {
    .bytofst = 5,
    .bitofst = 4,
};
// analog sticks
static const scalitm ds4lstickX = {
    .bytofst = 1,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4lstickY = {
    .bytofst = 2,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4rstickX = {
    .bytofst = 3,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4rstickY = {
    .bytofst = 4,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const boolitm ds4L3 = {
    .bytofst = 6,
    .bitofst = 6,
};
static const boolitm ds4R3 = {
    .bytofst = 6,
    .bitofst = 7,
};
// shoulder buttons
static const boolitm ds4L1 = {
    .bytofst = 6,
    .bitofst = 0,
};
static const boolitm ds4R1 = {
    .bytofst = 6,
    .bitofst = 1,
};
static const boolitm ds4L2bool = {
    .bytofst = 6,
    .bitofst = 2,
};
static const boolitm ds4R2bool = {
    .bytofst = 6,
    .bitofst = 3,
};
static const scalitm ds4L2 = {
    .bytofst = 8,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4R2 = {
    .bytofst = 9,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
// share, option and PS button
static const boolitm ds4share = {
    .bytofst = 6,
    .bitofst = 5,
};
static const boolitm ds4optns = {
    .bytofst = 6,
    .bitofst = 4,
};
static const boolitm ds4ps = {
    .bytofst = 7,
    .bitofst = 0,
};
// trackpad
static const scalitm ds4tpadtime = {
    .bytofst = 34,
};
static const boolitm ds4tpadclick = {
    .bytofst = 7,
    .bitofst = 1,
};
static const boolitm ds4tpadf1touch = {
    .bytofst = 35,
    .bitofst = 1,
};
static const boolitm ds4tpadf2touch = {
    .bytofst = 39,
    .bitofst = 1,
};
// sensors
static const scalitm ds4gyroX = {
    .bytofst = 13,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4gyroY = {
    .bytofst = 15,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4gyroZ = {
    .bytofst = 17,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4accelX = {
    .bytofst = 19,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4accelY = {
    .bytofst = 21,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4accelZ = {
    .bytofst = 23,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
/* End: DS4 report items */