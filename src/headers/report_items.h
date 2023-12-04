#include <stdint.h>
#include <limits.h>

typedef struct{
    uint8_t bytofst;
    uint8_t bitofst;
} boolitm;
typedef struct {
    uint8_t bytofst;
    uint8_t nbyts;
    uint8_t s; // 1 for signed, 0 for unsigned
    int32_t min;
    int32_t max;
} scalitm;

/* Start: DS4 report items */
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
static const boolitm ds4tpadclick = {
    .bytofst = 7,
    .bitofst = 1,
};
static const scalitm ds4ltpadX = {
    .bytofst = 34,
    .nbyts = 2,
};
static const scalitm ds4ltpadY = {
    .bytofst = 36,
    .nbyts = 2,
};
static const scalitm ds4rtpadX = {
    .bytofst = 38,
    .nbyts = 2,
};
static const scalitm ds4rtpadY = {
    .bytofst = 40,
    .nbyts = 2,
};
// sensors
static const scalitm ds4gyroX = {
    .bytofst = 13,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm ds4gyroY = {
    .bytofst = 15,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm ds4gyroZ = {
    .bytofst = 17,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
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
/* Start: DS4 report items */

/* Start: SDC report items */
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
static const scalitm sdcltpadX = {
    .bytofst = 17,
    .nbyts = 1,
};
static const scalitm sdcltpadY = {
    .bytofst = 19,
    .nbyts = 1,
};
static const scalitm sdcrtpadX = {
    .bytofst = 21,
    .nbyts = 1,
};
static const scalitm sdcrtpadY = {
    .bytofst = 23,
    .nbyts = 1,
};
// sensors
static const scalitm sdcgyroX = {
    .bytofst = 31,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm sdcgyroY = {
    .bytofst = 33,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm sdcgyroZ = {
    .bytofst = 35,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm sdcaccelX = {
    .bytofst = 25,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcaccelY = {
    .bytofst = 27,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdcaccelZ = {
    .bytofst = 29,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
/* End: SDC report items */