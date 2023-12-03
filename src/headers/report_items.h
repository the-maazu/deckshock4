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
static const scalitm ds4_counter = {
    .bytofst = 7,
    .nbyts = 1,
};
static const scalitm ds4_batlvl = {
    .bytofst = 12,
    .nbyts = 1,
};
static const scalitm ds4_tymstmp = {
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
static const scalitm ds4_dpad = {
    .bytofst = 5,
    .nbyts = 1,
};
// face buttons
static const boolitm ds4_btn_X = {
    .bytofst = 5,
    .bitofst = 5,
};
static const boolitm ds4_btn_O = {
    .bytofst = 5,
    .bitofst = 6,
};
static const boolitm ds4_btn_Tri = {
    .bytofst = 5,
    .bitofst = 7,
};
static const boolitm ds4_btn_Sq = {
    .bytofst = 5,
    .bitofst = 4,
};
// analog sticks
static const scalitm ds4_lstick_X = {
    .bytofst = 1,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4_lstick_Y = {
    .bytofst = 2,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4_rstick_X = {
    .bytofst = 3,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4_rstick_Y = {
    .bytofst = 4,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const boolitm ds4_L3 = {
    .bytofst = 6,
    .bitofst = 6,
};
static const boolitm ds4_R3 = {
    .bytofst = 6,
    .bitofst = 7,
};
// shoulder buttons
static const boolitm ds4_L1 = {
    .bytofst = 6,
    .bitofst = 0,
};
static const boolitm ds4_R1 = {
    .bytofst = 6,
    .bitofst = 1,
};
static const boolitm ds4_L2_bool = {
    .bytofst = 6,
    .bitofst = 2,
};
static const boolitm ds4_R2_bool = {
    .bytofst = 6,
    .bitofst = 3,
};
static const scalitm ds4_L2 = {
    .bytofst = 8,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
static const scalitm ds4_R2 = {
    .bytofst = 9,
    .nbyts = 1,
    .s = 0,
    .min = 0,
    .max = UINT8_MAX,
};
// share, option and PS button
static const boolitm ds4_share = {
    .bytofst = 6,
    .bitofst = 5,
};
static const boolitm ds4_options = {
    .bytofst = 6,
    .bitofst = 4,
};
static const boolitm ds4_PS = {
    .bytofst = 7,
    .bitofst = 0,
};
// trackpad
static const boolitm ds4_tpad_click = {
    .bytofst = 7,
    .bitofst = 1,
};
static const scalitm ds4_ltpad_X = {
    .bytofst = 34,
    .nbyts = 2,
};
static const scalitm ds4_ltpad_Y = {
    .bytofst = 36,
    .nbyts = 2,
};
static const scalitm ds4_rtpad_X = {
    .bytofst = 38,
    .nbyts = 2,
};
static const scalitm ds4_rtpad_Y = {
    .bytofst = 40,
    .nbyts = 2,
};
// sensors
static const scalitm ds4_gyroX = {
    .bytofst = 13,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm ds4_gyroY = {
    .bytofst = 15,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm ds4_gyroZ = {
    .bytofst = 17,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm ds4_accelX = {
    .bytofst = 19,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4_accelY = {
    .bytofst = 21,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm ds4_accelZ = {
    .bytofst = 23,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
/* Start: DS4 report items */

/* Start: SDC report items */
//  dpad
static const boolitm sdc_dpad_down = {
    .bytofst = 9,
    .bitofst = 3,
};
static const boolitm sdc_dpad_left = {
    .bytofst = 9,
    .bitofst = 2,
};
static const boolitm sdc_dpad_right = {
    .bytofst = 9,
    .bitofst = 1,
};
static const boolitm sdc_dpad_up = {
    .bytofst = 9,
    .bitofst = 0,
};
// face buttons
static const boolitm sdc_btn_A = {
    .bytofst = 8,
    .bitofst = 7,
};
static const boolitm sdc_btn_X = {
    .bytofst = 8,
    .bitofst = 6,
};
static const boolitm sdc_btn_B = {
    .bytofst = 8,
    .bitofst = 5,
};
static const boolitm sdc_btn_Y = {
    .bytofst = 8,
    .bitofst = 4,
};

// analog sticks
static const scalitm sdc_lstick_X = {
    .bytofst = 48,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdc_lstick_Y = {
    .bytofst = 50,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdc_rstick_X = {
    .bytofst = 52,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdc_rstick_Y = {
    .bytofst = 54,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const boolitm sdc_L3 = {
    .bytofst = 10,
    .bitofst = 6,
};
static const boolitm sdc_R3 = {
    .bytofst = 11,
    .bitofst = 2,
};
// shoulder buttons
static const boolitm sdc_L1 = {
    .bytofst = 8,
    .bitofst = 3,
};
static const boolitm sdc_R1 = {
    .bytofst = 8,
    .bitofst = 2,
};
static const boolitm sdc_L2_bool = {
    .bytofst = 8,
    .bitofst = 1,
};
static const boolitm sdc_R2_bool = {
    .bytofst = 8,
    .bitofst = 0,
};
static const scalitm sdc_L2 = {
    .bytofst = 44,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = 0x7FFF,
};
static const scalitm sdc_R2 = {
    .bytofst = 46,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = 0x7FFF,
};
// share, option and STEAM button
static const boolitm sdc_menu = {
    .bytofst = 9,
    .bitofst = 4,
};
static const boolitm sdc_options = {
    .bytofst = 9,
    .bitofst = 6,
};
static const boolitm sdc_steam = {
    .bytofst = 9,
    .bitofst = 5,
};
// trackpad
static const boolitm sdc_tpad_click_left = {
    .bytofst = 10,
    .bitofst = 1,
};
static const boolitm sdc_tpad_click_right = {
    .bytofst = 10,
    .bitofst = 2,
};
static const scalitm sdc_tpad_left_X = {
    .bytofst = 17,
    .nbyts = 1,
};
static const scalitm sdc_tpad_left_Y = {
    .bytofst = 19,
    .nbyts = 1,
};
static const scalitm sdc_tpad_right_X = {
    .bytofst = 21,
    .nbyts = 1,
};
static const scalitm sdc_tpad_right_Y = {
    .bytofst = 23,
    .nbyts = 1,
};
// sensors
static const scalitm sdc_gyroX = {
    .bytofst = 31,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm sdc_gyroY = {
    .bytofst = 33,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm sdc_gyroZ = {
    .bytofst = 35,
    .nbyts = 2,
    .s = 0,
    .min = 0,
    .max = UINT16_MAX,
};
static const scalitm sdc_accel_X = {
    .bytofst = 25,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdc_accel_Y = {
    .bytofst = 27,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
static const scalitm sdc_accel_Z = {
    .bytofst = 29,
    .nbyts = 2,
    .s = 1,
    .min = INT16_MIN,
    .max = INT16_MAX,
};
/* End: SDC report items */