#include <linux/uhid.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/sdc.h"
#include "headers/ds4.h"
#include <time.h>
#include "headers/reportconv.h"
#include <poll.h>
#include <unistd.h>

#define SDC_REP_SIZE 64
#define DS4_REP_SIZE 63

int main(int argc, char **argv)
{
    char sdc_rep[SDC_REP_SIZE], ds4_rep[DS4_REP_SIZE];

    fputs("Starting\n", stdout);
    if (sdc_open() == EXIT_FAILURE)
    {
        fputs("Failed to open SDC\n", stdout);
        return EXIT_FAILURE;
    }
    fputs("Opened SDC Successfully\n", stdout);
    if (ds4_create() == EXIT_FAILURE)
    {
        fputs("Failed to create DS4\n", stdout);
        return EXIT_FAILURE;
    }
    fputs("Created DS4 Successfully\n", stdout);

    // sdc read test 64 bytes
    // while (1)
    // {
    //     size = sdc_read_report(sdc_buf, sizeof(sdc_buf));
    //     fprintf(stdout, "size: %i :: ", size);  
    //     for( int i = 0; i < size; ++i)
    //     {
    //         fprintf(stdout, "%02hhx, ", sdc_buf[i]);   
    //     }
    //     fputs("\n", stdout);
    // }

    // ds4 test
    // ds4_create();
    // ds4_destroy();

    int msec = 0, trigger = 50000; /* 10ms */
    clock_t before = clock();
    do {
        ds4_recieve_req();
        memset(ds4_rep, 0, SDC_REP_SIZE);
        memset(sdc_rep, 0, SDC_REP_SIZE);

        sdc_read_report(sdc_rep, sizeof(sdc_rep));
        report_conv_sdc_to_ds4(sdc_rep, ds4_rep);
        // ds4_send_report(ds4_rep, DS4_REP_SIZE);

        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < trigger );
    
    return EXIT_SUCCESS;
}