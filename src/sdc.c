#include <systemd/sd-device.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int sdc_fd;

int sdc_close()
{
    fputs("Closing SDC\n", stderr);
    return close(sdc_fd);
}

int sdc_open()
{
    const char *path[12];
    // vendor defined usage.
    static char desc_tip[3] = "\x06\xFF\xFF";
    sd_device *device;
    static sd_device_enumerator *sdc_enum;
    struct hidraw_devinfo devinfo;
    struct hidraw_report_descriptor desc;

    sd_device_enumerator_new(&sdc_enum);
    sd_device_enumerator_ref(sdc_enum);
    sd_device_enumerator_add_match_subsystem(sdc_enum, "hidraw", 1);

    device = sd_device_enumerator_get_device_first(sdc_enum);
    uint8_t i = 30; // go through a max of 30 devices
    while (i--)
    {
        sd_device_get_devname(device, path);
        sdc_fd = open(*path, O_RDWR | __O_CLOEXEC);
        if (sdc_fd < 0){
            fputs("Could not locate file\n", stderr);
            sd_device_enumerator_unref(sdc_enum);
            return EXIT_FAILURE;
        }
            
        ioctl(sdc_fd, HIDIOCGRAWINFO, &devinfo);
        ioctl(sdc_fd, HIDIOCGRDESCSIZE, &(desc.size));
        ioctl(sdc_fd, HIDIOCGRDESC, &desc);

        fprintf(stderr, "path: %s\n", *path);
        fprintf(stderr, "Bustype(3 for USB):%i VID:%x PID:%x First3bytes:0x",
        devinfo.bustype, devinfo.vendor, devinfo.product);
        for (i = 0; i < 3; i++)
			fprintf(stderr, "%02x", desc.value[i]);
        fputs("\n", stderr);

        if (
            devinfo.bustype == BUS_USB 
            && devinfo.vendor == 0x28de 
            && devinfo.product == 0x1205 
            && !memcmp(desc_tip, desc.value, sizeof(desc_tip))
        ) {
            sd_device_enumerator_unref(sdc_enum);
            return EXIT_SUCCESS;
        }

        device = sd_device_enumerator_get_device_next(sdc_enum);
    }

    sd_device_enumerator_unref(sdc_enum);
    return EXIT_FAILURE;
}

int sdc_read_report(char* buf, size_t nbyts)
{
    return read(sdc_fd, buf, nbyts);
}

static void handle_inhibit(int bool)
{
    const char *path[100];
    int fd;
    sd_device *device;
    static sd_device_enumerator *enumtr;
    struct input_id inputid;

    sd_device_enumerator_new(&enumtr);
    sd_device_enumerator_ref(enumtr);
    sd_device_enumerator_add_match_subsystem(enumtr, "input", 1);

    device = sd_device_enumerator_get_device_first(enumtr);
    uint8_t i = 50;
    while (i--)
    {
        sd_device_get_devname(device, path);
        fd = open(*path, O_RDWR | __O_CLOEXEC);
        if (fd < 0)
        {
            device = sd_device_enumerator_get_device_next(enumtr);
            fprintf(stderr, "Could not locate file at %s\n", *path);
            continue;
        }
            

        ioctl(fd, EVIOCGID, &inputid);
        if (
            inputid.vendor == 0x28de 
            || inputid.product == 0x1205 
        ) 
        {
            if (bool)
                fprintf(stderr, "inhibiting: %s\n", *path);
            else
                fprintf(stderr, "uninhibiting: %s\n", *path);
            ioctl(fd, EVIOCGRAB, bool);
        }
        device = sd_device_enumerator_get_device_next(enumtr);
    }
    sd_device_enumerator_unref(enumtr);

    // valve virtual jostick
    if (bool)
            fprintf(stderr, "inhibiting: /dev/input/js0\n");
        else
            fprintf(stderr, "uninhibiting: /dev/input/js0\n");
    fd = open("/dev/input/js0", O_RDWR | __O_CLOEXEC);
    ioctl(fd, EVIOCGRAB, bool);
}

int sdc_inhibit()
{
    handle_inhibit(1);
}

int sdc_uninhibit()
{
    handle_inhibit(0);
}