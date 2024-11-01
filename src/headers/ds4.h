#ifndef DS4
#define DS4
#include <stdlib.h>

int ds4_create();
void ds4_destroy();
int ds4_send_report(const char *rep, size_t nbyts);
int ds4_recieve_req();

#endif