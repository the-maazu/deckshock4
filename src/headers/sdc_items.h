#ifndef SDC_ITEMS
#define SDC_ITEMS

#include "rep_items.h"

//  dpad
static const boolitm sdcdpadD = {
    .bytofst = 9,
    .bitofst = 3,
};
static const boolitm sdcdpadL = {
    .bytofst = 9,
    .bitofst = 2,
};
static const boolitm sdcdpadR = {
    .bytofst = 9,
    .bitofst = 1,
};
static const boolitm sdcdpadU = {
    .bytofst = 9,
    .bitofst = 0,
};
// face buttons
static const boolitm sdcA = {
    .bytofst = 8,
    .bitofst = 7,
};
static const boolitm sdcX = {
    .bytofst = 8,
    .bitofst = 6,
};
static const boolitm sdcB = {
    .bytofst = 8,
    .bitofst = 5,
};
static const boolitm sdcY = {
    .bytofst = 8,
    .bitofst = 4,
};

// analog sticks
static const scalitm sdclstickX = {
    .bytofst = 48,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdclstickY = {
    .bytofst = 50,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcrstickX = {
    .bytofst = 52,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcrstickY = {
    .bytofst = 54,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const boolitm sdcL3 = {
    .bytofst = 10,
    .bitofst = 6,
};
static const boolitm sdcR3 = {
    .bytofst = 11,
    .bitofst = 2,
};
// shoulder buttons
static const boolitm sdcL1 = {
    .bytofst = 8,
    .bitofst = 3,
};
static const boolitm sdcR1 = {
    .bytofst = 8,
    .bitofst = 2,
};
static const boolitm sdcL2bool = {
    .bytofst = 8,
    .bitofst = 1,
};
static const boolitm sdcR2bool = {
    .bytofst = 8,
    .bitofst = 0,
};
static const scalitm sdcL2 = {
    .bytofst = 44,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = 0x7FFF,
};
static const scalitm sdcR2 = {
    .bytofst = 46,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = 0x7FFF,
};
// share, option and STEAM button
static const boolitm sdcmenu = {
    .bytofst = 9,
    .bitofst = 4,
};
static const boolitm sdcoptns = {
    .bytofst = 9,
    .bitofst = 6,
};
static const boolitm sdcSTEAM = {
    .bytofst = 9,
    .bitofst = 5,
};
// trackpad
static const boolitm sdcltpadclick = {
    .bytofst = 10,
    .bitofst = 1,
};
static const boolitm sdcrtpadclick = {
    .bytofst = 10,
    .bitofst = 2,
};
static const boolitm sdcltpadtouch = {
    .bytofst = 10,
    .bitofst = 3,
};
static const boolitm sdcrtpadtouch = {
    .bytofst = 10,
    .bitofst = 4,
};
static const scalitm sdcltpadX = {
    .bytofst = 16,
    .nbyts = 2,
};
static const scalitm sdcltpadY = {
    .bytofst = 18,
    .nbyts = 1,
};
static const scalitm sdcrtpadX = {
    .bytofst = 20,
    .nbyts = 1,
};
static const scalitm sdcrtpadY = {
    .bytofst = 22,
    .nbyts = 1,
};
// sensors
static const scalitm sdcgyroP = {
    .bytofst = 30,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcgyroY = {
    .bytofst = 32,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcgyroR = {
    .bytofst = 34,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcaccelX = {
    .bytofst = 24,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcaccelY = {
    .bytofst = 26,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcaccelZ = {
    .bytofst = 28,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};

#endif