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


rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello");

  char s[100]="hello RTEMS";

  raspberrypi_spi_init(raspberrypi_SPI0);

  int fd = open("/dev/spidev0", O_RDWR);
  if (fd < 0) {
      printf("can't open device \n");
  } else {
    printf("SPI - Open Succeed. Start Init SPI...n\n");
  }
  while(1)
  {
    int ret = write(fd, s, 100);
    if (ret < 0) {
      printf("SPI Write error "); 
    }
    sleep(1);
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