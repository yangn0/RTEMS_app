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
#include <bsp/console.h>
// #include <bsp/aux.h>
#include <bsp/fatal.h>

rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello");
  char s[10];

  struct termios term;
  // tcgetattr(STDIN_FILENO, &term);
  // tcsetattr(STDIN_FILENO, TCSANOW, &term);

  // printf("scanf: ");
  // scanf("%s",s);
  // printf("printf: %s\n",s);

  // for (int i = 0;i < 100;i++) {
  //   printf("hello%d\n", i);
    
  // }

  raspberrypi_uart_init(UART2);

  int devfd = open("/dev/ttyAMA1", O_RDWR);

  tcgetattr(devfd, &term);
  tcsetattr(devfd, TCSANOW, &term);

  write(devfd, "\n read:", strlen("\n read:"));
  int nread=read(devfd,s,10);
  write(devfd,s,strlen(s));

  puts("end");

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