#define _POSIX_C_SOURCE 199309L

#include <linux/uhid.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
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

void *ds4_input_routine()
{
    while (1)
    {
        ds4_recieve_req();
    }
    
}

void *steam_btn_routine(void * sdcrep)
{
    struct timespec prevtp;
    struct timespec curtp;
    clock_gettime(CLOCK_REALTIME, &prevtp);
    curtp = prevtp;

    while(1) 
    {
        if(curtp.tv_sec - prevtp.tv_sec > 5)
            quit(); // quit if steam button held for 5 secs

        if(sdc_steam_down(sdcrep)) // update current time
            clock_gettime(CLOCK_REALTIME, &curtp);
        else 
        { // reset time delta
            clock_gettime(CLOCK_REALTIME, &prevtp);
            curtp = prevtp;
        }
    }
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

    pthread_t steam_btn_thread, ds4_input_thread;
    pthread_create(&steam_btn_thread, NULL, steam_btn_routine, sdcrep);
    pthread_create(&ds4_input_thread, NULL, ds4_input_routine, NULL);
    
    sdc_inhibit();
    while(1) 
    {   
        sdc_read_report(sdcrep, sizeof(sdcrep));
        trans_rep_sdc_to_ds4(ds4rep, sdcrep);
        ds4_send_report(ds4rep, REP_SIZE);
    }
}