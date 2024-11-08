#include <stdint.h>
#include <sys/types.h>

int sdc_open();
int sdc_close();
int sdc_read_report(char* buf, size_t nbyts);
int sdc_vgp_grab();
int sdc_vgp_release();
uint8_t sdc_steam_down(const char* buf);