#include <systemd/sd-device.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/items.h"

static int sdcfd;
static int sdcvgpfd;

int sdc_close()
{
    fputs("Closing SDC\n", stderr);
    return close(sdcfd);
}

int sdc_open()
{
    const char *path;
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
        sd_device_get_devname(device, &path);
        sdcfd = open(path, O_RDWR | __O_CLOEXEC);
        if (sdcfd < 0){
            fputs("Could not locate file\n", stderr);
            sd_device_enumerator_unref(sdcenum);
            return EXIT_FAILURE;
        }
            
        ioctl(sdcfd, HIDIOCGRAWINFO, &devinfo);
        ioctl(sdcfd, HIDIOCGRDESCSIZE, &(desc.size));
        ioctl(sdcfd, HIDIOCGRDESC, &desc);

        if (
            devinfo.bustype == BUS_USB 
            && devinfo.vendor == 0x28de 
            && devinfo.product == 0x1205 
            && !memcmp(desc_tip, desc.value, sizeof(desc_tip))
        ) {
            sd_device_enumerator_unref(sdcenum);
            fputs("Opened SDC successfully\n", stderr);
            return EXIT_SUCCESS;
        }
    }

    sd_device_enumerator_unref(sdcenum);
    fputs("Failed to open SDC\n", stderr);
    return EXIT_FAILURE;
}

int sdc_vgp_grab()
{
    char *path;
    char *product;
    char *name;
    sd_device *device;
    sd_device *parent;
    static sd_device_enumerator *sdcenum;

    sd_device_enumerator_new(&sdcenum);
    sd_device_enumerator_ref(sdcenum);
    sd_device_enumerator_add_match_subsystem(sdcenum, "input", 1);

    device = sd_device_enumerator_get_device_first(sdcenum);
    while ((device = sd_device_enumerator_get_device_next(sdcenum)))
    {
        sd_device_get_parent(device, &parent);
        sd_device_get_property_value( parent, "PRODUCT", (const char **) &product);
        
        if(strcmp("3/28de/11ff/1", (const char *) product) == 0){
            sd_device_get_property_value( parent, "NAME", (const char **) &name);
            sd_device_get_devname(device, (const char **) &path);
            fprintf(stderr, "Found SDC virtual input PRODUCT=%s NAME=%s at %s\n", product, name, path);

            sdcvgpfd = open(path, O_RDONLY);
            if (sdcvgpfd < 0)
                continue;
            ioctl(sdcvgpfd, EVIOCGRAB, 1);
            fputs("Grabbed(inhibiting) SDC virtual input device\n", stderr);
            sd_device_enumerator_unref(sdcenum);
            return EXIT_SUCCESS;
        }
    }

    sd_device_enumerator_unref(sdcenum);
    return EXIT_FAILURE;
}

int sdc_vgp_release()
{
    ioctl(sdcvgpfd, EVIOCGRAB, 0);
    fputs("Released SDC virtual input device\n", stderr);
    return close(sdcvgpfd);
}

int sdc_read_report(char* buf, size_t nbyts)
{
    return read(sdcfd, buf, nbyts);
}

uint8_t sdc_steam_down(const char* buf)
{
    return (buf[sdcSTEAM.byte] >> sdcSTEAM.bit) & 1;
}