#ifndef TRANS
#define TRANS

#define REP_SIZE 64
void trans_init();
void trans_config_check();
void trans_rep_sdc_to_ds4(char *ds4_report, const char *sdc_report);
void trans_deinit();
#endif