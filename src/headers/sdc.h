#include <stdint.h>

int sdc_open();
int sdc_close();
int sdc_read_report(char* buf, size_t nbyts);
uint8_t sdc_steam_down(const char* buf);