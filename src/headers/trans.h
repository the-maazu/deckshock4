#ifndef TRANS
#define TRANS

#include <stdint.h>
#define REP_SIZE 64
void trans_init();
uint8_t trans_is_disabled();
void trans_config_probe();
void trans_rep_sdc_to_ds4(char *ds4_report, const char *sdc_report);
void trans_deinit();
#endif