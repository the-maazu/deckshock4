#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/uhid.h>
#include <errno.h>
#include <poll.h>

// descriptor
static const char ds4_rdesc[] = {
    0x05, 0x01,                    // Usage Page (Generic Desktop)        0
    0x09, 0x05,                    // Usage (Game Pad)                    2
    0xa1, 0x01,                    // Collection (Application)            4
    0x85, 0x01,                    //  Report ID (1)                      6
    0x09, 0x30,                    //  Usage (X)                          8
    0x09, 0x31,                    //  Usage (Y)                          10
    0x09, 0x32,                    //  Usage (Z)                          12
    0x09, 0x35,                    //  Usage (Rz)                         14
    0x15, 0x00,                    //  Logical Minimum (0)                16
    0x26, 0xff, 0x00,              //  Logical Maximum (255)              18
    0x75, 0x08,                    //  Report Size (8)                    21
    0x95, 0x04,                    //  Report Count (4)                   23
    0x81, 0x02,                    //  Input (Data,Var,Abs)               25
    0x09, 0x39,                    //  Usage (Hat switch)                 27
    0x15, 0x00,                    //  Logical Minimum (0)                29
    0x25, 0x07,                    //  Logical Maximum (7)                31
    0x35, 0x00,                    //  Physical Minimum (0)               33
    0x46, 0x3b, 0x01,              //  Physical Maximum (315)             35
    0x65, 0x14,                    //  Unit (EnglishRotation: deg)        38
    0x75, 0x04,                    //  Report Size (4)                    40
    0x95, 0x01,                    //  Report Count (1)                   42
    0x81, 0x42,                    //  Input (Data,Var,Abs,Null)          44
    0x65, 0x00,                    //  Unit (None)                        46
    0x05, 0x09,                    //  Usage Page (Button)                48
    0x19, 0x01,                    //  Usage Minimum (1)                  50
    0x29, 0x0e,                    //  Usage Maximum (14)                 52
    0x15, 0x00,                    //  Logical Minimum (0)                54
    0x25, 0x01,                    //  Logical Maximum (1)                56
    0x75, 0x01,                    //  Report Size (1)                    58
    0x95, 0x0e,                    //  Report Count (14)                  60
    0x81, 0x02,                    //  Input (Data,Var,Abs)               62
    0x06, 0x00, 0xff,              //  Usage Page (Vendor Defined Page 1) 64
    0x09, 0x20,                    //  Usage (Vendor Usage 0x20)          67
    0x75, 0x06,                    //  Report Size (6)                    69
    0x95, 0x01,                    //  Report Count (1)                   71
    0x15, 0x00,                    //  Logical Minimum (0)                73
    0x25, 0x7f,                    //  Logical Maximum (127)              75
    0x81, 0x02,                    //  Input (Data,Var,Abs)               77
    0x05, 0x01,                    //  Usage Page (Generic Desktop)       79
    0x09, 0x33,                    //  Usage (Rx)                         81
    0x09, 0x34,                    //  Usage (Ry)                         83
    0x15, 0x00,                    //  Logical Minimum (0)                85
    0x26, 0xff, 0x00,              //  Logical Maximum (255)              87
    0x75, 0x08,                    //  Report Size (8)                    90
    0x95, 0x02,                    //  Report Count (2)                   92
    0x81, 0x02,                    //  Input (Data,Var,Abs)               94
    0x06, 0x00, 0xff,              //  Usage Page (Vendor Defined Page 1) 96
    0x09, 0x21,                    //  Usage (Vendor Usage 0x21)          99
    0x95, 0x36,                    //  Report Count (54)                  101
    0x81, 0x02,                    //  Input (Data,Var,Abs)               103
    0x85, 0x05,                    //  Report ID (5)                      105
    0x09, 0x22,                    //  Usage (Vendor Usage 0x22)          107
    0x95, 0x1f,                    //  Report Count (31)                  109
    0x91, 0x02,                    //  Output (Data,Var,Abs)              111
    0x85, 0x04,                    //  Report ID (4)                      113
    0x09, 0x23,                    //  Usage (Vendor Usage 0x23)          115
    0x95, 0x24,                    //  Report Count (36)                  117
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             119
    0x85, 0x02,                    //  Report ID (2)                      121
    0x09, 0x24,                    //  Usage (Vendor Usage 0x24)          123
    0x95, 0x24,                    //  Report Count (36)                  125
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             127
    0x85, 0x08,                    //  Report ID (8)                      129
    0x09, 0x25,                    //  Usage (Vendor Usage 0x25)          131
    0x95, 0x03,                    //  Report Count (3)                   133
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             135
    0x85, 0x10,                    //  Report ID (16)                     137
    0x09, 0x26,                    //  Usage (Vendor Usage 0x26)          139
    0x95, 0x04,                    //  Report Count (4)                   141
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             143
    0x85, 0x11,                    //  Report ID (17)                     145
    0x09, 0x27,                    //  Usage (Vendor Usage 0x27)          147
    0x95, 0x02,                    //  Report Count (2)                   149
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             151
    0x85, 0x12,                    //  Report ID (18)                     153
    0x06, 0x02, 0xff,              //  Usage Page (Vendor Usage Page 0xff02) 155
    0x09, 0x21,                    //  Usage (Vendor Usage 0x21)          158
    0x95, 0x0f,                    //  Report Count (15)                  160
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             162
    0x85, 0x13,                    //  Report ID (19)                     164
    0x09, 0x22,                    //  Usage (Vendor Usage 0x22)          166
    0x95, 0x16,                    //  Report Count (22)                  168
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             170
    0x85, 0x14,                    //  Report ID (20)                     172
    0x06, 0x05, 0xff,              //  Usage Page (Vendor Usage Page 0xff05) 174
    0x09, 0x20,                    //  Usage (Vendor Usage 0x20)          177
    0x95, 0x10,                    //  Report Count (16)                  179
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             181
    0x85, 0x15,                    //  Report ID (21)                     183
    0x09, 0x21,                    //  Usage (Vendor Usage 0x21)          185
    0x95, 0x2c,                    //  Report Count (44)                  187
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             189
    0x06, 0x80, 0xff,              //  Usage Page (Vendor Usage Page 0xff80) 191
    0x85, 0x80,                    //  Report ID (128)                    194
    0x09, 0x20,                    //  Usage (Vendor Usage 0x20)          196
    0x95, 0x06,                    //  Report Count (6)                   198
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             200
    0x85, 0x81,                    //  Report ID (129)                    202
    0x09, 0x21,                    //  Usage (Vendor Usage 0x21)          204
    0x95, 0x06,                    //  Report Count (6)                   206
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             208
    0x85, 0x82,                    //  Report ID (130)                    210
    0x09, 0x22,                    //  Usage (Vendor Usage 0x22)          212
    0x95, 0x05,                    //  Report Count (5)                   214
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             216
    0x85, 0x83,                    //  Report ID (131)                    218
    0x09, 0x23,                    //  Usage (Vendor Usage 0x23)          220
    0x95, 0x01,                    //  Report Count (1)                   222
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             224
    0x85, 0x84,                    //  Report ID (132)                    226
    0x09, 0x24,                    //  Usage (Vendor Usage 0x24)          228
    0x95, 0x04,                    //  Report Count (4)                   230
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             232
    0x85, 0x85,                    //  Report ID (133)                    234
    0x09, 0x25,                    //  Usage (Vendor Usage 0x25)          236
    0x95, 0x06,                    //  Report Count (6)                   238
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             240
    0x85, 0x86,                    //  Report ID (134)                    242
    0x09, 0x26,                    //  Usage (Vendor Usage 0x26)          244
    0x95, 0x06,                    //  Report Count (6)                   246
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             248
    0x85, 0x87,                    //  Report ID (135)                    250
    0x09, 0x27,                    //  Usage (Vendor Usage 0x27)          252
    0x95, 0x23,                    //  Report Count (35)                  254
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             256
    0x85, 0x88,                    //  Report ID (136)                    258
    0x09, 0x28,                    //  Usage (Vendor Usage 0x28)          260
    0x95, 0x22,                    //  Report Count (34)                  262
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             264
    0x85, 0x89,                    //  Report ID (137)                    266
    0x09, 0x29,                    //  Usage (Vendor Usage 0x29)          268
    0x95, 0x02,                    //  Report Count (2)                   270
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             272
    0x85, 0x90,                    //  Report ID (144)                    274
    0x09, 0x30,                    //  Usage (Vendor Usage 0x30)          276
    0x95, 0x05,                    //  Report Count (5)                   278
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             280
    0x85, 0x91,                    //  Report ID (145)                    282
    0x09, 0x31,                    //  Usage (Vendor Usage 0x31)          284
    0x95, 0x03,                    //  Report Count (3)                   286
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             288
    0x85, 0x92,                    //  Report ID (146)                    290
    0x09, 0x32,                    //  Usage (Vendor Usage 0x32)          292
    0x95, 0x03,                    //  Report Count (3)                   294
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             296
    0x85, 0x93,                    //  Report ID (147)                    298
    0x09, 0x33,                    //  Usage (Vendor Usage 0x33)          300
    0x95, 0x0c,                    //  Report Count (12)                  302
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             304
    0x85, 0xa0,                    //  Report ID (160)                    306
    0x09, 0x40,                    //  Usage (Vendor Usage 0x40)          308
    0x95, 0x06,                    //  Report Count (6)                   310
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             312
    0x85, 0xa1,                    //  Report ID (161)                    314
    0x09, 0x41,                    //  Usage (Vendor Usage 0x41)          316
    0x95, 0x01,                    //  Report Count (1)                   318
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             320
    0x85, 0xa2,                    //  Report ID (162)                    322
    0x09, 0x42,                    //  Usage (Vendor Usage 0x42)          324
    0x95, 0x01,                    //  Report Count (1)                   326
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             328
    0x85, 0xa3,                    //  Report ID (163)                    330
    0x09, 0x43,                    //  Usage (Vendor Usage 0x43)          332
    0x95, 0x30,                    //  Report Count (48)                  334
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             336
    0x85, 0xa4,                    //  Report ID (164)                    338
    0x09, 0x44,                    //  Usage (Vendor Usage 0x44)          340
    0x95, 0x0d,                    //  Report Count (13)                  342
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             344
    0x85, 0xa5,                    //  Report ID (165)                    346
    0x09, 0x45,                    //  Usage (Vendor Usage 0x45)          348
    0x95, 0x15,                    //  Report Count (21)                  350
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             352
    0x85, 0xa6,                    //  Report ID (166)                    354
    0x09, 0x46,                    //  Usage (Vendor Usage 0x46)          356
    0x95, 0x15,                    //  Report Count (21)                  358
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             360
    0x85, 0xf0,                    //  Report ID (240)                    362
    0x09, 0x47,                    //  Usage (Vendor Usage 0x47)          364
    0x95, 0x3f,                    //  Report Count (63)                  366
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             368
    0x85, 0xf1,                    //  Report ID (241)                    370
    0x09, 0x48,                    //  Usage (Vendor Usage 0x48)          372
    0x95, 0x3f,                    //  Report Count (63)                  374
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             376
    0x85, 0xf2,                    //  Report ID (242)                    378
    0x09, 0x49,                    //  Usage (Vendor Usage 0x49)          380
    0x95, 0x0f,                    //  Report Count (15)                  382
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             384
    0x85, 0xa7,                    //  Report ID (167)                    386
    0x09, 0x4a,                    //  Usage (Vendor Usage 0x4a)          388
    0x95, 0x01,                    //  Report Count (1)                   390
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             392
    0x85, 0xa8,                    //  Report ID (168)                    394
    0x09, 0x4b,                    //  Usage (Vendor Usage 0x4b)          396
    0x95, 0x01,                    //  Report Count (1)                   398
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             400
    0x85, 0xa9,                    //  Report ID (169)                    402
    0x09, 0x4c,                    //  Usage (Vendor Usage 0x4c)          404
    0x95, 0x08,                    //  Report Count (8)                   406
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             408
    0x85, 0xaa,                    //  Report ID (170)                    410
    0x09, 0x4e,                    //  Usage (Vendor Usage 0x4e)          412
    0x95, 0x01,                    //  Report Count (1)                   414
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             416
    0x85, 0xab,                    //  Report ID (171)                    418
    0x09, 0x4f,                    //  Usage (Vendor Usage 0x4f)          420
    0x95, 0x39,                    //  Report Count (57)                  422
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             424
    0x85, 0xac,                    //  Report ID (172)                    426
    0x09, 0x50,                    //  Usage (Vendor Usage 0x50)          428
    0x95, 0x39,                    //  Report Count (57)                  430
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             432
    0x85, 0xad,                    //  Report ID (173)                    434
    0x09, 0x51,                    //  Usage (Vendor Usage 0x51)          436
    0x95, 0x0b,                    //  Report Count (11)                  438
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             440
    0x85, 0xae,                    //  Report ID (174)                    442
    0x09, 0x52,                    //  Usage (Vendor Usage 0x52)          444
    0x95, 0x01,                    //  Report Count (1)                   446
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             448
    0x85, 0xaf,                    //  Report ID (175)                    450
    0x09, 0x53,                    //  Usage (Vendor Usage 0x53)          452
    0x95, 0x02,                    //  Report Count (2)                   454
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             456
    0x85, 0xb0,                    //  Report ID (176)                    458
    0x09, 0x54,                    //  Usage (Vendor Usage 0x54)          460
    0x95, 0x3f,                    //  Report Count (63)                  462
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             464
    0x85, 0xb1,                    //  Report ID (177)                    466
    0x09, 0x55,                    //  Usage (Vendor Usage 0x55)          468
    0x95, 0x02,                    //  Report Count (2)                   470
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             472
    0x85, 0xb2,                    //  Report ID (178)                    474
    0x09, 0x56,                    //  Usage (Vendor Usage 0x56)          476
    0x95, 0x02,                    //  Report Count (2)                   478
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             480
    0x85, 0xb3,                    //  Report ID (179)                    482
    0x09, 0x55,                    //  Usage (Vendor Usage 0x55)          484
    0x95, 0x3f,                    //  Report Count (63)                  486
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             488
    0x85, 0xb4,                    //  Report ID (180)                    490
    0x09, 0x55,                    //  Usage (Vendor Usage 0x55)          492
    0x95, 0x3f,                    //  Report Count (63)                  494
    0xb1, 0x02,                    //  Feature (Data,Var,Abs)             496
    0xc0,                          // End Collection                      498
};

// reports below acquired from https://psdevwiki.com/ps4/DS4-USB
#define REPID_MAC 0x81
// static const char ds4_mac_rep[] = {0x81,0x8B,0x09,0x07,0x6D,0x66,0x1C};
static const char ds4_mac_rep[] = {0x81,0x56,0x78,0x90,0x5f,0x1b,0x40};

#define REPID_MAC2 0x12
// static const char ds4_mac_rep2[] = {
//     0x12,0x8B,0x09,0x07,
//     0x6D,0x66,0x1C,0x08,
//     0x25,0x00,0xAC,0x9E,
//     0x17,0x94,0x05,0xB0,
// };
static const char ds4_mac_rep2[] = {
    0x12,0x56,0x78,0x90,0x5f,0x1b,0x40,
    0x08,0x25,0x00,0x82,0xa1,0x71,0x65,
    0x5a,0x50
};


#define REPID_CALIB 0x02
// static const char ds4_calib_rep[] = {
//     0x02,0x01,0x00,0x00,0x00,0x00,
//     0x00,0x87,0x22,0x7B,0xDD,0xB2,
//     0x22,0x47,0xDD,0xBD,0x22,0x43,
//     0xDD,0x1C,0x02,0x1C,0x02,0x7F,
//     0x1E,0x2E,0xDF,0x60,0x1F,0x4C,
//     0xE0,0x3A,0x1D,0xC6,0xDE,0x08,
//     0x00
// };
static const char ds4_calib_rep[] = {
    0x02,0xff,0xff,0x09,0x00,0xfe,
    0xff,0xa2,0x22,0x56,0xdd,0xa4,
    0x22,0x73,0xdd,0xd0,0x23,0x26,
    0xdc,0x1c,0x02,0x1c,0x02,0xd4,
    0x20,0x2c,0xdf,0x42,0x20,0xbe,
    0xdf,0x36,0x1f,0xcb,0xe0,0x04,
    0x00
};

#define REPID_VERS 0xA3
// static const char ds4_ver_rep[] = {
//     0xA3, 0x41,0x75,0x67,0x20,0x20,0x33,0x20,0x32,
//     0x30,0x31,0x33,0x00,0x00,0x00,0x00,0x00,0x30,
//     0x37,0x3A,0x30,0x31,0x3A,0x31,0x32,0x00,0x00,
//     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
//     0x31,0x03,0x00,0x00,0x00,0x49,0x00,0x05,0x00,
//     0x00,0x80,0x03,0x00,
// };
static const char ds4_ver_rep[] = {
    0xa3,0x53,0x65,0x70,0x20,0x20,0x34,
    0x20,0x32,0x30,0x31,0x35,0x00,0x00,
    0x00,0x00,0x00,0x30,0x33,0x3a,0x35,
    0x36,0x3a,0x30,0x30,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,
    0x04,0x54,0x01,0x00,0x00,0x00,0x35,
    0x20,0x03,0x01,0x00,0x80,0x03,0x00
};

static int ds4_fd;
static struct pollfd ds4_pfd;

static int uhid_write( const struct uhid_event *ev)
{
	ssize_t ret;

	ret = write(ds4_fd, ev, sizeof(*ev));
	if (ret < 0) {
		fprintf(stderr, "Cannot write to uhid: %m\n");
		return -errno;
	} else if (ret != sizeof(*ev)) {
		fprintf(stderr, "Wrong size written to uhid: %zd != %zu\n",
			ret, sizeof(ev));
		return -EFAULT;
	} else {
		return 0;
	}
}

int ds4_create()
{
	const char *path = "/dev/uhid";
	struct uhid_event ev;
	int ret;

	fprintf(stderr, "Open uhid-cdev %s\n", path);
	ds4_fd = open(path, O_RDWR | __O_CLOEXEC);
	if (ds4_fd < 0) {
		fprintf(stderr, "Cannot open uhid-cdev %s: %m\n", path);
		return EXIT_FAILURE;
	}

	fprintf(stderr, "Create DS4 device\n");
	memset(&ev, 0, sizeof(ev));
	ev.type = UHID_CREATE2;
	strcpy((char*)ev.u.create2.name, "Wireless Controller");
	memcpy(ev.u.create2.rd_data, ds4_rdesc, sizeof(ds4_rdesc));
	ev.u.create2.rd_size = sizeof(ds4_rdesc);
	ev.u.create2.bus = BUS_USB;
	ev.u.create2.vendor = 0x054c;
	ev.u.create2.product = 0x05c4;
	ev.u.create2.version = 0x0100;
	ev.u.create2.country = 0;
    ds4_pfd.fd = ds4_fd;
    ds4_pfd.events = POLLIN;

	return uhid_write(&ev);
}

void ds4_destroy()
{
    fputs("Destroying DS4\n", stderr);
	struct uhid_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type = UHID_DESTROY;
	uhid_write(&ev);
}

int ds4_send_report(char *report, size_t nbyts)
{
	struct uhid_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type = UHID_INPUT2;
	ev.u.input2.size = nbyts;
    memcpy(ev.u.input2.data, report, nbyts);
	return uhid_write(&ev);
}

static void handle_get_report(const struct uhid_event *ev)
{
    struct uhid_event nev;
    nev.type = UHID_GET_REPORT_REPLY;
    nev.u.get_report_reply.id = ev->u.get_report.id;
    nev.u.get_report_reply.err = 0;

    fprintf(stderr, "Get report: %02x\n", ev->u.get_report.rnum);
    switch(ev->u.get_report.rnum)
    {
        case REPID_MAC:
            nev.u.get_report_reply.size = sizeof(ds4_mac_rep);
            memcpy(nev.u.get_report_reply.data, ds4_mac_rep, sizeof(ds4_mac_rep));
	        uhid_write(&nev);  
            break;
        case REPID_MAC2:
            nev.u.get_report_reply.size = sizeof(ds4_mac_rep2);
            memcpy(nev.u.get_report_reply.data, ds4_mac_rep2, sizeof(ds4_mac_rep2));
	        uhid_write(&nev);  
            break;
        case REPID_CALIB:
            nev.u.get_report_reply.size = sizeof(ds4_calib_rep);
            memcpy(nev.u.get_report_reply.data, ds4_calib_rep, sizeof(ds4_calib_rep));
	        uhid_write(&nev);
            break;
        case REPID_VERS:
            nev.u.get_report_reply.size = sizeof(ds4_ver_rep);
            memcpy(nev.u.get_report_reply.data, ds4_ver_rep, sizeof(ds4_ver_rep));
	        uhid_write(&nev);
            break;
    }
    
}

int ds4_recieve_req()
{
    poll(&ds4_pfd, 1, 0);
    if(!(ds4_pfd.revents & POLLIN)) 
        return 0;

	struct uhid_event ev;
	ssize_t ret;

	memset(&ev, 0, sizeof(ev));
	ret = read(ds4_fd, &ev, sizeof(ev));
	if (ret == 0) {
		fprintf(stderr, "Read HUP on uhid-cdev\n");
		return -EFAULT;
	} else if (ret < 0) {
		fprintf(stderr, "Cannot read uhid-cdev: %m\n");
		return -errno;
	} else if (ret != sizeof(ev)) {
		fprintf(stderr, "Invalid size read from uhid-dev: %zd != %zu\n",
			ret, sizeof(ev));
		return -EFAULT;
	}
	switch (ev.type) {
        case UHID_GET_REPORT:
            // fprintf(stderr, "UHID_GET_REPORT from uhid-dev\n");
            handle_get_report(&ev);
            break;
	    case UHID_START:
	    	// fprintf(stderr, "UHID_START from uhid-dev\n");
	    	break;
	    case UHID_STOP:
	    	// fprintf(stderr, "UHID_STOP from uhid-dev\n");
	    	break;
	    case UHID_OPEN:
	    	// fprintf(stderr, "UHID_OPEN from uhid-dev\n");
	    	break;
	    case UHID_CLOSE:
	    	// fprintf(stderr, "UHID_CLOSE from uhid-dev\n");
	    	break;
	    case UHID_OUTPUT:
	    	// fprintf(stderr, "UHID_OUTPUT from uhid-dev\n");
	    	break;
	    case UHID_OUTPUT_EV:
	    	// fprintf(stderr, "UHID_OUTPUT_EV from uhid-dev\n");
	    	break;
	    // default:
		// fprintf(stderr, "Invalid event from uhid-dev: %u\n", ev.type);
	}

	return 0;
}