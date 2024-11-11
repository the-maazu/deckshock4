#ifndef REPORT_ITEMS

#define REPORT_ITEMS
#include <stdint.h>
#include <limits.h>

typedef struct{
    uint8_t byte; // zero based byte offset 
    uint8_t bit; // zero based bit offset
} bool_item;

typedef struct {
    uint8_t byte; // byte offset
    uint8_t size;
    uint8_t s; // 1 for signed, 0 for unsigned
    int32_t min;
    int32_t max;
} scalar_item;

// bool or virtual item
enum ItemType {BItem, VItem};
enum VirtualItem {VShake};
typedef struct {
    enum ItemType type;
    union {
        bool_item* boolitm;
        enum VirtualItem virtitm;
    } value;
} bv_item;

#define BOOL_ITEM(val) (bv_item) {.type=BItem, .value.boolitm=val}
#define VIRT_ITEM(val) (bv_item) {.type=VItem, .value.virtitm=val}

// Start of SDC items
//  dpad
static const bool_item sdcdpadD = {
    .byte = 9,
    .bit = 3,
};
static const bool_item sdcdpadL = {
    .byte = 9,
    .bit = 2,
};
static const bool_item sdcdpadR = {
    .byte = 9,
    .bit = 1,
};
static const bool_item sdcdpadU = {
    .byte = 9,
    .bit = 0,
};
// face buttons
static const bool_item sdcA = {
    .byte = 8,
    .bit = 7,
};
static const bool_item sdcX = {
    .byte = 8,
    .bit = 6,
};
static const bool_item sdcB = {
    .byte = 8,
    .bit = 5,
};
static const bool_item sdcY = {
    .byte = 8,
    .bit = 4,
};

// analog sticks
static const scalar_item sdclstickX = {
    .byte = 48,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdclstickY = {
    .byte = 50,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcrstickX = {
    .byte = 52,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcrstickY = {
    .byte = 54,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const bool_item sdcL3 = {
    .byte = 10,
    .bit = 6,
};
static const bool_item sdcR3 = {
    .byte = 11,
    .bit = 2,
};
// shoulder buttons
static const bool_item sdcL1 = {
    .byte = 8,
    .bit = 3,
};
static const bool_item sdcR1 = {
    .byte = 8,
    .bit = 2,
};
static const bool_item sdcL2bool = {
    .byte = 8,
    .bit = 1,
};
static const bool_item sdcR2bool = {
    .byte = 8,
    .bit = 0,
};
static const scalar_item sdcL2 = {
    .byte = 44,
    .size = 2,
    .s = 0,
    .min = 0,
    .max = 0x7FFF,
};
static const scalar_item sdcR2 = {
    .byte = 46,
    .size = 2,
    .s = 0,
    .min = 0,
    .max = 0x7FFF,
};
// back buttons
static const bool_item sdcL4 = {
    .byte = 13,
    .bit = 1,
};
static const bool_item sdcL5 = {
    .byte = 9,
    .bit = 7,
};
static const bool_item sdcR4 = {
    .byte = 13,
    .bit = 2,
};
static const bool_item sdcR5 = {
    .byte = 10,
    .bit = 0,
};
// share, option and STEAM button
static const bool_item sdcmenu = {
    .byte = 9,
    .bit = 4,
};
static const bool_item sdcoptns = {
    .byte = 9,
    .bit = 6,
};
static const bool_item sdcSTEAM = {
    .byte = 9,
    .bit = 5,
};
// trackpad
static const bool_item sdcltpadclick = {
    .byte = 10,
    .bit = 1,
};
static const bool_item sdcrtpadclick = {
    .byte = 10,
    .bit = 2,
};
static const bool_item sdcltpadtouch = {
    .byte = 10,
    .bit = 3,
};
static const bool_item sdcrtpadtouch = {
    .byte = 10,
    .bit = 4,
};
static const scalar_item sdcltpadX = {
    .byte = 16,
    .size = 2,
};
static const scalar_item sdcltpadY = {
    .byte = 18,
    .size = 1,
};
static const scalar_item sdcrtpadX = {
    .byte = 20,
    .size = 1,
};
static const scalar_item sdcrtpadY = {
    .byte = 22,
    .size = 1,
};
// sensors
static const scalar_item sdcgyroP = {
    .byte = 30,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcgyroY = {
    .byte = 32,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcgyroR = {
    .byte = 34,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcaccelX = {
    .byte = 24,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcaccelY = {
    .byte = 26,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item sdcaccelZ = {
    .byte = 28,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
// End of SDC items


// Start of DS4 items
// time, counter and battery
static const scalar_item ds4counter = {
    .byte = 7,
    .size = 1,
};
static const scalar_item ds4batlvl = {
    .byte = 12,
    .size = 1,
};
static const scalar_item ds4tymstmp = {
    .byte = 10,
    .size = 2,
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
static const scalar_item ds4dpad = {
    .byte = 5,
    .size = 1,
};
// face buttons
static const bool_item ds4X = {
    .byte = 5,
    .bit = 5,
};
static const bool_item ds4O = {
    .byte = 5,
    .bit = 6,
};
static const bool_item ds4Tri = {
    .byte = 5,
    .bit = 7,
};
static const bool_item ds4Sq = {
    .byte = 5,
    .bit = 4,
};
// analog sticks
static const scalar_item ds4lstickX = {
    .byte = 1,
    .size = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalar_item ds4lstickY = {
    .byte = 2,
    .size = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalar_item ds4rstickX = {
    .byte = 3,
    .size = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalar_item ds4rstickY = {
    .byte = 4,
    .size = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const bool_item ds4L3 = {
    .byte = 6,
    .bit = 6,
};
static const bool_item ds4R3 = {
    .byte = 6,
    .bit = 7,
};
// shoulder buttons
static const bool_item ds4L1 = {
    .byte = 6,
    .bit = 0,
};
static const bool_item ds4R1 = {
    .byte = 6,
    .bit = 1,
};
static const bool_item ds4L2bool = {
    .byte = 6,
    .bit = 2,
};
static const bool_item ds4R2bool = {
    .byte = 6,
    .bit = 3,
};
static const scalar_item ds4L2 = {
    .byte = 8,
    .size = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalar_item ds4R2 = {
    .byte = 9,
    .size = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
// share, option and PS button
static const bool_item ds4share = {
    .byte = 6,
    .bit = 5,
};
static const bool_item ds4optns = {
    .byte = 6,
    .bit = 4,
};
static const bool_item ds4ps = {
    .byte = 7,
    .bit = 0,
};
// trackpad
static const scalar_item ds4tpadtime = {
    .byte = 34,
};
static const bool_item ds4tpadclick = {
    .byte = 7,
    .bit = 1,
};
static const bool_item ds4tpadf1touch = {
    .byte = 35,
    .bit = 1,
};
static const bool_item ds4tpadf2touch = {
    .byte = 39,
    .bit = 1,
};
// sensors
static const scalar_item ds4gyroX = {
    .byte = 13,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item ds4gyroY = {
    .byte = 15,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item ds4gyroZ = {
    .byte = 17,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item ds4accelX = {
    .byte = 19,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item ds4accelY = {
    .byte = 21,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalar_item ds4accelZ = {
    .byte = 23,
    .size = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
// End of DS4 items
#endif