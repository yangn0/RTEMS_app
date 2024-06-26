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
#include <bsp/console.h>
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

static rtems_status_code init_gpio()
{
    gpio_set_function(7, GPIO_AF0);      //CS1
    gpio_set_pull(8, GPIO_PULL_NONE);    //CS0
    gpio_set_function(9, GPIO_AF0);      //MISO 
    gpio_set_function(10, GPIO_AF0);     //MOSI
    gpio_set_function(11, GPIO_AF0);     //SCLK
}


static const char *device = "/dev/spi0";
static uint8_t mode = 3; /* SPI通信使用全双工，设置CPOL＝0，CPHA＝0。 */
static uint8_t bits = 8; /* ８ｂiｔｓ读写，MSB first。*/
static uint32_t speed = 100 * 1000;/* 设置0.5M传输速度 */
static int g_SPI_Fd = 0;


/**
* 功 能：关闭SPI模块
*/
int SPI_Close(void)
{
    int fd = g_SPI_Fd;


    if (fd == 0) /* SPI是否已经打开*/
    return 0;
    close(fd);
    g_SPI_Fd = 0;


    return 0;
}

int SPI_Write(uint8_t *TxBuf, int len)
{
    int ret;
    int fd = g_SPI_Fd;

    // printf("fd : %d\n",fd);
    ret = write(fd, TxBuf, len);
    if (ret < 0)
    printf("SPI Write errorn");

    return ret;
}

int SPI_Open(void)
{
    int fd;
    int ret = 0;

	printf("open spi dev:%s \r\n", device);
    fd = open(device, O_RDWR);
    if (fd < 0)
	{
        	printf("can't open device \n");
		return -1;
	}
    else
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

    g_SPI_Fd=fd;
    return ret;
}

#define SSD1306_RES_PIN 17
#define SSD1306_DC_PIN 22
#define SSD1306_CS_PIN 10

#define SSD1306_RESET_LOW     gpio_clear_pin (SSD1306_RES_PIN)
#define SSD1306_RESET_HIGH    gpio_set_pin (SSD1306_RES_PIN)
#define SSD1306_DC_LOW gpio_clear_pin (SSD1306_DC_PIN)
#define SSD1306_DC_HIGH gpio_set_pin (SSD1306_DC_PIN)
#define SSD1306_CS_LOW gpio_clear_pin (SSD1306_CS_PIN)
#define SSD1306_CS_HIGH gpio_set_pin (SSD1306_CS_PIN)

#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64

#define SSD1306_COLOR_BLACK 0x00
#define SSD1306_COLOR_WHITE 0x01

/* Private SSD1306 structure */
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

/* Private variable */
static SSD1306_t SSD1306;

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer_all[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void SSD1306_WriteCommand(uint8_t command)
{
    SSD1306_DC_LOW;
    SSD1306_CS_LOW;
    SPI_Write(&command,1);
    SSD1306_CS_HIGH;
    SSD1306_DC_HIGH;
}

void SSD1306_Fill(uint8_t color)
{
    if (SSD1306.Inverted)
    {
        color = (uint8_t)!color;
    }
    /* Set memory */
    memset(SSD1306_Buffer_all, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
}

void SPI_Write_Buf(uint8_t SSD1306_Buffer_all[],int n)
{
    int i=0;
    for(i=0;i<n;i++){
        SPI_Write(&SSD1306_Buffer_all[i],1);
    }
}

void SSD1306_UpdateScreen(void) 
{
    SSD1306_DC_HIGH;
    SSD1306_CS_LOW;
    SPI_Write_Buf(SSD1306_Buffer_all, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
    SSD1306_CS_HIGH;
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, uint8_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        /* Error */
        return;
    }

    /* Check if pixels are inverted */
    if (SSD1306.Inverted)
    {
        color = (uint8_t)!color;
    }

    /* Set color */
    if (color == SSD1306_COLOR_WHITE)
    {
        SSD1306_Buffer_all[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer_all[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c)
{
    int16_t dx, dy, sx, sy, err, e2, i, tmp; 
    
    /* Check for overflow */
    if (x0 >= SSD1306_WIDTH)
    {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH)
    {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT)
    {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT)
    {
        y1 = SSD1306_HEIGHT - 1;
    }
    
    dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
    dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
    sx = (x0 < x1) ? 1 : -1; 
    sy = (y0 < y1) ? 1 : -1; 
    err = ((dx > dy) ? dx : -dy) / 2; 

    if (dx == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }
        
        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }
        
        /* Vertical line */
        for (i = y0; i <= y1; i++)
        {
            SSD1306_DrawPixel(x0, i, c);
        }
        
        /* Return from function */
        return;
    }
    
    if (dy == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }
        
        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }
        
        /* Horizontal line */
        for (i = x0; i <= x1; i++)
        {
            SSD1306_DrawPixel(i, y0, c);
        }
        
        /* Return from function */
        return;
    }

    while (1)
    {
        SSD1306_DrawPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void SSD1306_GotoXY(uint16_t x, uint16_t y)
{
    /* Set write pointers */
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

void worker()
{
	
	char send_data[64] = {0};
	char read_data[64] = {0};
	char crc = 0;
	int i = 0;	
	int j = 0;

	/* spi 初始化程序 */
	  SPI_Open();
    SSD1306_RESET_LOW;
    sleep(0.005);
    SSD1306_RESET_HIGH;
    sleep(0.005);

    /* Init LCD */
    SSD1306_WriteCommand(0xAE); //display off
    /**
     * Set the lower start column address of pointer by command 00h~0Fh.
     * Set the upper start column address of pointer by command 10h~1Fh.
     */
    SSD1306_WriteCommand(0x00); //---set low column address
    SSD1306_WriteCommand(0x10); //---set high column address

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

    /* Clear screen */
    SSD1306_Fill(SSD1306_COLOR_WHITE);

    /* Update screen */
    SSD1306_UpdateScreen();

    // /* Set default values */
    // SSD1306.CurrentX = 0;
    // SSD1306.CurrentY = 0;

    // /* Initialized OK */
    // SSD1306.Initialized = 1;

    // SSD1306_DrawLine(0,   0, 127,  0, 1);
    // SSD1306_DrawLine(0,   0,   0, 63, 1);
    // SSD1306_DrawLine(127, 0, 127, 63, 1);
    // SSD1306_DrawLine(0,  63, 127, 63, 1);
    // SSD1306_UpdateScreen(); // display
}

rtems_task Init(
  rtems_task_argument ignored)
{
  puts("hello RTEMS");

  int r = raspberrypi_spi_init("/dev/spi0", (uintptr_t *) BCM2711_SPI0_BASE);
  
  worker();

  puts("end");
  exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
// #define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#define CONFIGURE_FILESYSTEM_IMFS
/* configuration for shell test */
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

// #define CONFIGURE_SHELL_COMMANDS_INIT
// #define CONFIGURE_SHELL_COMMANDS_ALL
// #include <rtems/shellconfig.h>