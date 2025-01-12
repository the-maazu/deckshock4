#define _POSIX_C_SOURCE 199309L

#include <linux/uhid.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "headers/sdc.h"
#include "headers/ds4.h"
#include "headers/trans.h"

void quit(int status)
{
    fputs("\nQuitting\n", stderr);
    ds4_destroy();
    sdc_close();
    sdc_vgp_release();
    trans_deinit();
    exit(status);
}

int main(int argc, char **argv)
{
    char sdcrep[REP_SIZE], ds4rep[REP_SIZE];
    signal(SIGINT, quit);
    signal(SIGKILL, quit);
    signal(SIGTERM, quit);
    signal(SIGQUIT, quit);

    fputs("Starting\n", stderr);

    if (
        sdc_open() == EXIT_FAILURE
        || ds4_create() == EXIT_FAILURE
    ) quit(EXIT_FAILURE);
    sdc_vgp_grab();
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
        trans_config_probe();
        if(trans_is_disabled())
        {   
            fputs("Disabling virtual controller\n", stderr);
            ds4_destroy();
            sdc_vgp_release();
            while(trans_is_disabled())
            {
                sleep(1); // throttle probe
                trans_config_probe();
            }
            ds4_create();
            sdc_vgp_grab();
        }
            
        
        nanosleep(&throttle, NULL);

        // steam button routine
        if(curtp.tv_sec - prevtp.tv_sec > 10)
            quit(EXIT_SUCCESS); // quit if steam button held for 10 secs
        clock_gettime(CLOCK_REALTIME, &curtp);            
        if(!sdc_steam_down(sdcrep))
        { // reset time delta
            clock_gettime(CLOCK_REALTIME, &prevtp);
            curtp = prevtp;
        }
        
        // translation
        ds4_recieve_req();
        sdc_read_report(sdcrep, sizeof(sdcrep));
        trans_rep_sdc_to_ds4(ds4rep, sdcrep);
        ds4_send_report(ds4rep, REP_SIZE);
    }
}