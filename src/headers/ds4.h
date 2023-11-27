int ds4_create();
void ds4_destroy();
int ds4_send_report(const char *report, size_t nbytes);
int ds4_recieve_req();