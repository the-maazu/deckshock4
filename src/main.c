#define _POSIX_C_SOURCE 199309L

#include <linux/uhid.h>
#include <unistd.h>
#include <signal.h>
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
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    char sdcrep[REP_SIZE], ds4rep[REP_SIZE];
    signal(SIGINT, quit);
    signal(SIGKILL, quit);
    signal(SIGTERM, quit);
    signal(SIGQUIT, quit);

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

    trans_init();
    
    // should help smoothen sensors
    const struct timespec throttle = {
        .tv_sec = 0,
        .tv_nsec = 1250000, // 1.25ms
    };

    struct timespec prevtp;
    struct timespec curtp;
    clock_gettime(CLOCK_REALTIME, &prevtp);
    curtp = prevtp;

    while(1) 
    {   
        nanosleep(&throttle, NULL);
        ds4_recieve_req();
        sdc_read_report(sdcrep, sizeof(sdcrep));

        if(curtp.tv_sec - prevtp.tv_sec > 10)
            quit(); // quit if steam button held for 10 secs

        if(sdc_steam_down(sdcrep)) // update current time
            clock_gettime(CLOCK_REALTIME, &curtp);
        else 
        { // reset time delta
            clock_gettime(CLOCK_REALTIME, &prevtp);
            curtp = prevtp;
        }

        trans_rep_sdc_to_ds4(ds4rep, sdcrep);
        ds4_send_report(ds4rep, REP_SIZE);
    }
}