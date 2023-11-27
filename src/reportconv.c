#include <stdint.h>
#include <stdio.h>

typedef enum {digital, analog} item_type;

typedef struct{
    uint8_t byte_offset;
    uint8_t bit_offset;
} digital_item;

typedef struct {
    uint8_t byte_offset;
    uint8_t nbits;
} analog_item;

typedef struct {
    item_type type;
    union {
        digital_item digi;
        analog_item ana;
    } data;
} report_item;

/* DS4 report items */
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

const analog_item ds4_dpad = {
    .byte_offset = 4,
    .nbits = 4,
};

// face buttons
const digital_item ds4_btn_X = {
    .byte_offset = 4,
    .bit_offset = 5,
};

const digital_item ds4_btn_O = {
    .byte_offset = 4,
    .bit_offset = 6,
};

const digital_item ds4_btn_Tri = {
    .byte_offset = 4,
    .bit_offset = 7,
};

const digital_item ds4_btn_Sq = {
    .byte_offset = 4,
    .bit_offset = 4,
};

// analog_item sticks
const analog_item ds4_left_stick_X = {
    .byte_offset = 0,
    .nbits = 8,
};

const analog_item ds4_left_stick_Y = {
    .byte_offset = 0,
    .nbits = 8,
};

const analog_item ds4_right_stick_X = {
    .byte_offset = 2,
    .nbits = 8,
};

const analog_item ds4_right_stick_Y = {
    .byte_offset = 3,
    .nbits = 8,
};


const digital_item ds4_L3 = {
    .byte_offset = 5,
    .bit_offset = 6,
};

const digital_item ds4_R3 = {
    .byte_offset = 5,
    .bit_offset = 7,
};

// shoulder buttons
const digital_item ds4_L1 = {
    .byte_offset = 5,
    .bit_offset = 0,
};

const digital_item ds4_R1 = {
    .byte_offset = 5,
    .bit_offset = 1,
};

const digital_item ds4_L2_digital = {
    .byte_offset = 5,
    .bit_offset = 2,
};

const digital_item ds4_R2_digital = {
    .byte_offset = 5,
    .bit_offset = 3,
};

const analog_item ds4_L2 = {
    .byte_offset = 7,
    .nbits = 8,
};

const analog_item ds4_R2 = {
    .byte_offset = 8,
    .nbits = 8,
};

// share, option and PS button
const digital_item ds4_share = {
    .byte_offset = 5,
    .bit_offset = 5,
};

const digital_item ds4_options = {
    .byte_offset = 5,
    .bit_offset = 5,
};

const digital_item ds4_PS = {
    .byte_offset = 6,
    .bit_offset = 0,
};


// trackpad
const digital_item ds4_tpad_click = {
    .byte_offset = 6,
    .bit_offset = 1,
};

const analog_item ds4_tpad_left_X = {
    .byte_offset = 34,
    .nbits = 16,
};

const analog_item ds4_tpad_left_Y = {
    .byte_offset = 36,
    .nbits = 16,
};

const analog_item ds4_tpad_right_X = {
    .byte_offset = 38,
    .nbits = 16,
};

const analog_item ds4_tpad_right_Y = {
    .byte_offset = 40,
    .nbits = 16,
};

// sensors
const analog_item ds4_gyro_X = {
    .byte_offset = 12,
    .nbits = 16,
};

const analog_item ds4_gyro_Y = {
    .byte_offset = 14,
    .nbits = 16,
};

const analog_item ds4_gyro_Z = {
    .byte_offset = 16,
    .nbits = 16,
};

const analog_item ds4_accel_X = {
    .byte_offset = 18,
    .nbits = 16,
};

const analog_item ds4_accel_Y = {
    .byte_offset = 20,
    .nbits = 16,
};

const analog_item ds4_accel_Z = {
    .byte_offset = 22,
    .nbits = 16,
};

/* SDC report items */
//  dpad
const digital_item sdc_dpad_down = {
    .byte_offset = 9,
    .bit_offset = 3,
};

const digital_item sdc_dpad_left = {
    .byte_offset = 9,
    .bit_offset = 2,
};

const digital_item sdc_dpad_right = {
    .byte_offset = 9,
    .bit_offset = 1,
};

const digital_item sdc_dpad_up = {
    .byte_offset = 9,
    .bit_offset = 0,
};

// face buttons
const digital_item sdc_btn_A = {
    .byte_offset = 10,
    .bit_offset = 7,
};

const digital_item sdc_btn_X = {
    .byte_offset = 10,
    .bit_offset = 6,
};

const digital_item sdc_btn_B = {
    .byte_offset = 10,
    .bit_offset = 5,
};

const digital_item sdc_btn_Y = {
    .byte_offset = 10,
    .bit_offset = 4,
};

// analog_item sticks
const analog_item sdc_left_stick_X = {
    .byte_offset = 49,
    .nbits = 8,
};

const analog_item sdc_left_stick_Y = {
    .byte_offset = 51,
    .nbits = 8,
};

const analog_item sdc_right_stick_X = {
    .byte_offset = 53,
    .nbits = 8,
};

const analog_item sdc_right_stick_Y = {
    .byte_offset = 55,
    .nbits = 8,
};

const digital_item sdc_L3 = {
    .byte_offset = 11,
    .bit_offset = 6,
};

const digital_item sdc_R3 = {
    .byte_offset = 12,
    .bit_offset = 2,
};

// shoulder buttons
const digital_item sdc_L1 = {
    .byte_offset = 10,
    .bit_offset = 3,
};

const digital_item sdc_R1 = {
    .byte_offset = 10,
    .bit_offset = 2,
};

const digital_item sdc_L2_digital = {
    .byte_offset = 10,
    .bit_offset = 1,
};

const digital_item sdc_R2_digital = {
    .byte_offset = 10,
    .bit_offset = 1,
};

const analog_item sdc_L2 = {
    .byte_offset = 45,
    .nbits = 8,
};

const analog_item sdc_R2 = {
    .byte_offset = 47,
    .nbits = 8,
};

// share, option and PS button
const digital_item sdc_menu = {
    .byte_offset = 9,
    .bit_offset = 4,
};

const digital_item sdc_options = {
    .byte_offset = 9,
    .bit_offset = 6,
};

const digital_item sdc_steam = {
    .byte_offset = 9,
    .bit_offset = 5,
};

// trackpad
const digital_item sdc_tpad_click_left = {
    .byte_offset = 11,
    .bit_offset = 1,
};

const digital_item sdc_tpad_click_right = {
    .byte_offset = 11,
    .bit_offset = 2,
};

const analog_item sdc_tpad_left_X = {
    .byte_offset = 17,
    .nbits = 8,
};

const analog_item sdc_tpad_left_Y = {
    .byte_offset = 19,
    .nbits = 8,
};

const analog_item sdc_tpad_right_X = {
    .byte_offset = 21,
    .nbits = 8,
};

const analog_item sdc_tpad_right_Y = {
    .byte_offset = 23,
    .nbits = 8,
};

// sensors
const analog_item sdc_gyro_X = {
    .byte_offset = 31,
    .nbits = 16,
};

const analog_item sdc_gyro_Y = {
    .byte_offset = 33,
    .nbits = 16,
};

const analog_item sdc_gyro_Z = {
    .byte_offset = 35,
    .nbits = 16,
};

const analog_item sdc_accel_X = {
    .byte_offset = 25,
    .nbits = 16,
};

const analog_item sdc_accel_Y = {
    .byte_offset = 27,
    .nbits = 16,
};

const analog_item sdc_accel_Z = {
    .byte_offset = 29,
    .nbits = 16,
};


/* Functions */
static inline void reset_item(char * report, const report_item *item)
{
    if(item->type == digital)
        *(report + item->data.digi.bit_offset) &= ~(1 << item->data.digi.bit_offset);
    else
    {
        uint8_t shift = 8 - item->data.ana.nbits;
        *(report + item->data.digi.bit_offset) &= ~(0xFF << shift);
    }
}

static void conv_sdc_to_ds4_dpad(const char *sdc_report, char *ds4_report)
{
    uint8_t left = *(sdc_report + sdc_dpad_left.byte_offset) & (1 << sdc_dpad_left.bit_offset);
    uint8_t right = *(sdc_report + sdc_dpad_right.byte_offset) & (1 << sdc_dpad_right.bit_offset);
    uint8_t up = *(sdc_report + sdc_dpad_up.byte_offset) & (1 << sdc_dpad_up.bit_offset);
    uint8_t down = *(sdc_report + sdc_dpad_down.byte_offset) & (1 << sdc_dpad_down.bit_offset);

    uint8_t shift = 8 - ds4_dpad.nbits;
    const report_item ds4_dpad_item = {
        .type = analog,
        .data.ana = ds4_dpad,
    };

    reset_item(ds4_report, &ds4_dpad_item);
    if(left)
    {
        if(up)
            *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_NW << shift;
        else if(down)
            *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_SW << shift;
        else
            *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_W << shift;
    } 
    else if(right)
    {
        if(up)
            *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_NE << shift;
        else if(down)
            *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_SE << shift;
        else
            *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_E << shift;        
    }
    else if (up)
        *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_N << shift;
    else if (down)
        *(ds4_report + ds4_dpad.byte_offset) |= DS4_DPAD_N << shift;
    
}

int report_conv_sdc_to_ds4(const char *sdc_report, char *ds4_report)
{
    conv_sdc_to_ds4_dpad(sdc_report, ds4_report);
}

