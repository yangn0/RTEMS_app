/*
 * Hello world example
 */

#include <rtems.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <rtems/error.h>
#include <string.h>
#include <bsp/fatal.h>
#include <bsp/rpi-gpio.h>
#include <bsp/raspberrypi.h>
#include <bsp/spi.h>
#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <bsp.h>
#include <rtems/shell.h>
#include <bsp/rpi-gpio.h>
#include <linux/spi/spidev.h>

static const char *device = "/dev/spidev0";
static uint32_t mode = 3;
static uint8_t bits = 8;
static uint32_t speed = 100 * 1000;
static int g_SPI_Fd = 0;

int SPI_Write(uint8_t *TxBuf, int len)
{
    int ret;
    int fd = g_SPI_Fd;

    ret = write(fd, TxBuf, len);
    if (ret < 0)
        printf("SPI Write error ");

    return ret;
}

int SPI_Transfer(const uint8_t *TxBuf, uint8_t *RxBuf, int len)
{
    int ret;
    int fd = g_SPI_Fd;

    struct spi_ioc_transfer tr;
    memset(&tr, 0x00, sizeof(tr));
    tr.tx_buf = TxBuf;
    tr.rx_buf = RxBuf;
    tr.len = len;
    tr.speed_hz = speed;
    tr.bits_per_word = bits;
    tr.cs = 0;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        printf("%d can't send spi message", ret);
    }
    return 0;
}

int SPI_Open(void)
{
    int fd;
    int ret = 0;

    printf("open spi dev:%s \r\n", device);
    fd = open(device, O_RDWR);
    if (fd < 0) {
        printf("can't open device \n");
        return -1;
    } else
        printf("SPI - Open Succeed. Start Init SPI...n\n");

    /*
    * spi mode
    */
    ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
    if (ret == -1)
        printf("can't set spi mode\n");


    ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
    if (ret == -1)
        printf("can't get spi mode\n");


    /*
    * bits per word
    */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        printf("can't set bits per word\n");


    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        printf("can't get bits per word\n");


    /*
    * max speed hz
    */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        printf("can't set max speed hz\n");


    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        printf("can't get max speed hz\n");


    g_SPI_Fd = fd;
    return ret;
}

#define SSD1306_RES_PIN 17
#define SSD1306_DC_PIN  27

#define SSD1306_RESET_LOW     raspberrypi_gpio_clear_pin (SSD1306_RES_PIN)
#define SSD1306_RESET_HIGH    raspberrypi_gpio_set_pin (SSD1306_RES_PIN)
#define SSD1306_DC_LOW        raspberrypi_gpio_clear_pin (SSD1306_DC_PIN)
#define SSD1306_DC_HIGH       raspberrypi_gpio_set_pin (SSD1306_DC_PIN)

#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64

#define SSD1306_COLOR_BLACK 0x00
#define SSD1306_COLOR_WHITE 0x01

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer_all[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void SSD1306_WriteCommand(uint8_t command)
{
    SSD1306_DC_LOW;
    // SPI_Write(&command, 1);
    SPI_Transfer(&command, NULL, 1);
}

void SSD1306_Fill(uint8_t color)
{
    memset(SSD1306_Buffer_all, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
}

void SPI_Write_Buf(uint8_t SSD1306_Buffer_all[], int n)
{
    int i = 0;
    for (i = 0;i < n;i++) {
        SPI_Write(&SSD1306_Buffer_all[i], 1);
    }
}



void SSD1306_UpdateScreen(void)
{
    SSD1306_DC_HIGH;
    // SPI_Write_Buf(SSD1306_Buffer_all, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
    SPI_Transfer(SSD1306_Buffer_all, NULL, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
}

void ssd1306_init()
{
    SSD1306_RESET_LOW;
    sleep(0.5);
    SSD1306_RESET_HIGH;
    sleep(0.5);

    /* Init LCD */
    SSD1306_WriteCommand(0xAE); //display off
    SSD1306_WriteCommand(0xAE); //display off

    /**
     * Set the lower start column address of pointer by command 00h~0Fh.
     * Set the upper start column address of pointer by command 10h~1Fh.
     */
    SSD1306_WriteCommand(0x00); //---set low column address
    // SSD1306_WriteCommand(0x10); //---set high column address

    /** set contrast control register, 2 bytes, 0x00 - 0xFF */
    SSD1306_WriteCommand(0x81);
    SSD1306_WriteCommand(0x7F);
    /** 0xA4,Output follows RAM content
     *  0xA5,Output ignores RAM content */
    SSD1306_WriteCommand(0xA4);
    /** 0xA6, Normal display (RESET)
     *  0xA7, Inverse display */
    SSD1306_WriteCommand(0xA6);
    /* 0x20,Set Memory Addressing Mode, 2 bytes,
     *   0x00,Horizontal Addressing Mode (slide horizontally and goto next page)
     *   0x01,Vertical Addressing Mode (slide vertically and goto next column)
     *   0x02,Page Addressing Mode (RESET) (slide horizontally and remain in the same page)
     *   0x03,Invalid
    */
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x00);
    /**
     * Set the page start address of the target display location by command B0h to B7h
     * For Page Addressing Mode only
     */
    SSD1306_WriteCommand(0xB0);
    /**
     * Set Page Address, 3 bytes
     * For Horizontal and Vertical Addressing Mode only
     */
    SSD1306_WriteCommand(0x22);
    SSD1306_WriteCommand(0x00); // From Page 0
    SSD1306_WriteCommand(0x07); // To Page 7

    /**
     * COM Output Scan Direction
     * 0xC0: normal mode (RESET) Scan from COM0 to COM[N –1]
     * 0xC8: remapped mode. Scan from COM[N-1] to COM0 */
    SSD1306_WriteCommand(0xC8); //Set COM Output Scan Direction
    /**
     * Set display RAM display start line register from 0-63 */
    SSD1306_WriteCommand(0x40);
    /**
     * Segment Re-map
     * 0xA0: column address 0 is mapped to SEG0 (RESET),
     * 0xA1: column address 127 is mapped to SEG0 */
    SSD1306_WriteCommand(0xA1);
    /**
     * Set MUX ratio to N+1 MUX
     * N=A[5:0]: from 16MUX to 64MUX, RESET=111111b (i.e. 63d, 64MUX)
     * A[5:0] from 0 to 14 are invalid entry.*/
    SSD1306_WriteCommand(0xA8);
    SSD1306_WriteCommand(0x3F);
    /**
     * Set Display Offset, Set vertical shift by COM from 0d~63d
     * The value is reset to 00h after RESET */
    SSD1306_WriteCommand(0xD3);
    SSD1306_WriteCommand(0x00); // offset in vertical
    /**
     * Set COM Pins Hardware Configuration
     * A[4]=0b, Sequential COM pin configuration
     * A[4]=1b(RESET), Alternative COM pin configuration
     * A[5]=0b(RESET), Disable COM Left/Right remap
     * A[5]=1b, Enable COM Left/Right remap */
    SSD1306_WriteCommand(0xDA);
    SSD1306_WriteCommand(0x12); // A[4]=0, A[5]=1
    /**
     * Set Display Divide Ratio/Oscillator Frequency
     * */
    SSD1306_WriteCommand(0xD5);
    SSD1306_WriteCommand(0xF0); // divide ratio
    /**
     * Set Pre-charge Period */
    SSD1306_WriteCommand(0xD9);
    SSD1306_WriteCommand(0x22);
    /**
     * Set V COMH Deselect Level
     * 0x00: 0.65 * Vcc
     * 0x10: 0.77 * Vcc (RESET)
     * 0x11: 0.83 * Vcc
     * */
    SSD1306_WriteCommand(0xDB);
    SSD1306_WriteCommand(0x10);

    /** charge pump setting
     * 0x10: Disable charge pump(RESET)
     * 0x14: Enable charge pump during display on
     */
    SSD1306_WriteCommand(0x8D);
    SSD1306_WriteCommand(0x14);

    /** 0xAE, Display OFF (sleep mode),
     *  0xAF, Display ON in normal mode */
    SSD1306_WriteCommand(0xAF);

}

void ssd1306_RTEMS_logo()
{
    uint8_t temp[SSD1306_WIDTH * SSD1306_HEIGHT / 8] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x3F,0x9F,0x9F,0x8F,0x8F,0x87,0x87,0xC7,0x03,0x03,0x03,0x01,0x01,0x01,0x40,0x80,0x80,0x80,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x80,0x80,0x40,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x83,0x83,0x83,0x87,0x87,0x4F,0x8F,0x8F,0x5F,0x1F,0x3F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x0F,0x07,0x03,0x03,0x05,0x00,0x00,0x00,0x80,0x70,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7E,0x3E,0x7C,0x7E,0x7E,0x7E,0x7E,0x7D,0x7E,0x7E,0x7E,0x7F,0x7F,0x7D,0x7E,0x6E,0x7F,0x7F,0x3F,0x7E,0x7E,0x7E,0x7E,0x3C,0x7E,0x7E,0x7E,0x7F,0x7F,0x87,0x00,0x80,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x4F,0x7F,0x7F,0x3F,0x7F,0x7F,0x67,0x7E,0x7F,0x7F,0x7E,0x7E,0x3E,0x00,0x00,0x00,0x01,0x01,0x03,0x07,0x0F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x78,0x78,0xF8,0xF8,0xF8,0xF4,0x78,0x78,0x78,0xF8,0xF8,0xF8,0xF8,0x78,0x78,0x78,0xC0,0xF8,0xF8,0xF8,0xF8,0x78,0xF8,0x78,0x78,0x38,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x84,0xF0,0xF8,0xF8,0xF8,0xF8,0xD8,0xE4,0xF0,0xF8,0xF8,0xF8,0xF8,0xF8,0xF0,0xFC,0xBC,0x38,0x38,0x38,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xF0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFE,0xFF,0xFF,0xFF,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0xC0,0xFE,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0x30,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0x1F,0x79,0xFB,0xFF,0xF7,0xF7,0xFF,0xFF,0x7F,0x3E,0x5C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xC0,0xE0,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x30,0xF1,0xF0,0xF0,0xF0,0xF0,0xF0,0xF1,0xF1,0xF0,0xF0,0x70,0xF0,0xF0,0xF0,0xF0,0xF1,0xC0,0xF0,0xF0,0x30,0xF8,0xF8,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xD0,0xC0,0xE0,0xF0,0xF0,0xF0,0xF0,0xF0,0x30,0xF0,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xD0,0xE0,0xE0,0xF4,0xF8,0xFC,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFC,0xFC,0xFC,0xF8,0xF8,0xF9,0xF8,0xF1,0xF0,0xF0,0xF1,0xE0,0xE1,0xE1,0xE0,0xE1,0xE1,0xE0,0xE1,0xC0,0xC1,0xC1,0xC1,0xC0,0xC0,0xC1,0xC1,0xC0,0xC1,0xC1,0xC0,0xC1,0xC1,0xC1,0xC1,0xE1,0xE1,0xE1,0xE0,0xE0,0xE1,0xE0,0xE0,0xE1,0xF0,0xF1,0xF1,0xF0,0xFB,0xFB,0xFB,0xF9,0xFC,0xFC,0xFC,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };

    memcpy(SSD1306_Buffer_all, temp, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
}

void worker()
{
    SPI_Open();
    ssd1306_init();

    for (int i = 0; i < 5;i++) {
        printf("%d\n", i);
        /* Clear screen */
        SSD1306_Fill(SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();

        sleep(1);

        ssd1306_RTEMS_logo();
        /* Update screen */
        SSD1306_UpdateScreen();

        sleep(1);
    }

    close(g_SPI_Fd);
}

rtems_task Init(
    rtems_task_argument ignored)
{
    puts("hello RTEMS");

    raspberrypi_gpio_set_function(SSD1306_RES_PIN, GPIO_OUTPUT);
    raspberrypi_gpio_set_function(SSD1306_DC_PIN, GPIO_OUTPUT);

    int r = raspberrypi_spi_init(raspberrypi_SPI0);

    worker();

    puts("end");
    exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_MAXIMUM_TASKS 20
#define CONFIGURE_MAXIMUM_SEMAPHORES 20
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 20
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 20
#define CONFIGURE_STACK_CHECKER_ENABLED
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_EXTRA_TASK_STACKS (6 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MALLOC_STATISTICS
#define CONFIGURE_UNIFIED_WORK_AREAS

#include <rtems/confdefs.h>