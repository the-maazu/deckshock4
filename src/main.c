#include <linux/uhid.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/sdc.h"
#include "headers/ds4.h"
#include <time.h>
#include "headers/trans.h"
#include <poll.h>
#include <unistd.h>
#include <signal.h>

void wait(int trigger)
{
    int msec = 0;
    clock_t before = clock();
    do {
        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < trigger );
}

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
    fputs("Opened SDC Successfully\n", stderr);
    if (ds4_create() == EXIT_FAILURE)
    {
        fputs("Failed to create DS4\n", stderr);
        quit();
    }
    fputs("Created DS4 Successfully\n", stderr);

    sdc_inhibit();
    while(1) 
    {
        ds4_recieve_req();
        sdc_read_report(sdcrep, sizeof(sdcrep));
        trans_rep_sdc_to_ds4(ds4rep, sdcrep);
        ds4_send_report(ds4rep, REP_SIZE);
    }
}