#define _POSIX_C_SOURCE 199309L

#include <linux/uhid.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "headers/sdc.h"
#include "headers/ds4.h"
#include "headers/trans.h"

void quit()
{
    fputs("\nQuitting\n", stderr);
    ds4_destroy();
    sdc_close();
    sdc_uninhibit();
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    char sdcrep[REP_SIZE], ds4rep[REP_SIZE];
    signal(SIGINT, quit);

    fputs("Starting\n", stderr);
    if (sdc_open() == EXIT_FAILURE)
    {
        fputs("Failed to open SDC\n", stderr);
        quit();
    }
    fputs("Opened SDC successfully\n", stderr);
    if (ds4_create() == EXIT_FAILURE)
    {
        fputs("Failed to create DS4\n", stderr);
        quit();
    }
    fputs("Created DS4 successfully\n", stderr);
    sdc_inhibit();

    struct timespec prevtp = {};
    struct timespec curtp = {};

    while(1) 
    {
        if(curtp.tv_sec - prevtp.tv_sec > 5)
            quit(); // quit if steam button held for 5 secs
        
        ds4_recieve_req();
        sdc_read_report(sdcrep, sizeof(sdcrep));
        trans_rep_sdc_to_ds4(ds4rep, sdcrep);
        ds4_send_report(ds4rep, REP_SIZE);

        if(sdc_steam_down(sdcrep)) // update current time
            clock_gettime(CLOCK_REALTIME, &curtp);
        else 
        { // reset time delta
            clock_gettime(CLOCK_REALTIME, &prevtp);
            memcpy(&curtp, &prevtp, sizeof(prevtp));
        }
    }
}