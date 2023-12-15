#ifndef REPORT_ITEMS

#define REPORT_ITEMS
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

#endif