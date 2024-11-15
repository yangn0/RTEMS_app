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
#include <dirent.h>
#include <bsp/fatal.h>
#include <bsp/rpi-gpio.h>
#include <bsp/raspberrypi.h>
#include <bsp/raspberrypi-spi.h>
#include <linux/spi/spidev.h>

static uint8_t bits = 8;
static uint32_t speed = 100 * 1000;
static int g_SPI_Fd = 0;

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

rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello");

  char s[100]="hello RTEMS";

  raspberrypi_spi_init(raspberrypi_SPI0);

  int fd = open("/dev/spidev0", O_RDWR);
  g_SPI_Fd = fd;
  if (fd < 0) {
      printf("can't open device \n");
  } else {
    printf("SPI - Open Succeed. Start Init SPI...n\n");
  }
  
  while (1)
  {
    char rx_buf[100]={0};
    // int ret = write(fd, s, 100);
    int ret=SPI_Transfer(s, rx_buf, 100);
    if (ret < 0) {
      printf("SPI Write error "); 
    }

    // char rx_buf[100];
    // ret = read(fd, rx_buf, 100);
    printf("read: %s\n", rx_buf);

    // sleep(1);
  }

  puts("ends");
  rtems_shutdown_executive(0);
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