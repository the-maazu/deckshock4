#include <systemd/sd-device.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/sdc_items.h"

static int sdcfd;

int sdc_close()
{
    fputs("Closing SDC\n", stderr);
    return close(sdcfd);
}

int sdc_open()
{
    const char *path[256];
    // vendor defined usage.
    static char desc_tip[3] = "\x06\xFF\xFF";
    sd_device *device;
    static sd_device_enumerator *sdcenum;
    struct hidraw_devinfo devinfo;
    struct hidraw_report_descriptor desc;

    sd_device_enumerator_new(&sdcenum);
    sd_device_enumerator_ref(sdcenum);
    sd_device_enumerator_add_match_subsystem(sdcenum, "hidraw", 1);

    device = sd_device_enumerator_get_device_first(sdcenum);
    while ((device = sd_device_enumerator_get_device_next(sdcenum)))
    {
        sd_device_get_devname(device, path);
        sdcfd = open(*path, O_RDWR | __O_CLOEXEC);
        if (sdcfd < 0){
            fputs("Could not locate file\n", stderr);
            sd_device_enumerator_unref(sdcenum);
            return EXIT_FAILURE;
        }
            
        ioctl(sdcfd, HIDIOCGRAWINFO, &devinfo);
        ioctl(sdcfd, HIDIOCGRDESCSIZE, &(desc.size));
        ioctl(sdcfd, HIDIOCGRDESC, &desc);

        fprintf(stderr, "path: %s\n", *path);
        fprintf(stderr, "Bustype(3 for USB):%i VID:%x PID:%x First3bytes:0x",
        devinfo.bustype, devinfo.vendor, devinfo.product);
        for (int i = 0; i < 3; i++)
			fprintf(stderr, "%02x", desc.value[i]);
        fputs("\n", stderr);

        if (
            devinfo.bustype == BUS_USB 
            && devinfo.vendor == 0x28de 
            && devinfo.product == 0x1205 
            && !memcmp(desc_tip, desc.value, sizeof(desc_tip))
        ) {
            sd_device_enumerator_unref(sdcenum);
            return EXIT_SUCCESS;
        }
    }

    sd_device_enumerator_unref(sdcenum);
    return EXIT_FAILURE;
}

int sdc_read_report(char* buf, size_t nbyts)
{
    return read(sdcfd, buf, nbyts);
}

uint8_t sdc_steam_down(const char* buf)
{
    return (buf[sdcSTEAM.bytofst] >> sdcSTEAM.bitofst) & 1;
}