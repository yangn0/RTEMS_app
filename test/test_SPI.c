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
// #include <bsp/console.h>
// #include <bsp/aux.h>
#include <bsp/fatal.h>
#include <bsp/rpi-gpio.h>
#include <bsp/raspberrypi.h>
#include <bsp/watchdog.h>
// #include <bsp/spi.h>


rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello");
  // sleep(1);

  char s[10];
  // char s2[100]={0};
  
  // printf("fcntl: %lx\n",fcntl(devfd,F_GETFL,0));

  // printf("scanf: ");
  // scanf("%s",s);
  // printf("printf: %s",s);

  // struct termios term;
  // tcgetattr(devfd, &term);
  // tcsetattr(devfd, TCSANOW, &term);

  // write(devfd, "\n read:", strlen("\n read:"));
  // int nread=read(devfd,s2,10);
  // sprintf(s2,"\n nread:%d s:%s\n",nread,s);
  // sprintf(s2,"\n s:%c\n",s[0]);
  // write(devfd, s2, strlen(s2));

  // rtems_shell_wait_for_input(
  //   devfd,
  //   20,
  //   notification,
  //   NULL
  // );

  // write(devfd, "\n read:", strlen("\n read:"));
  // int nread=read(devfd,s,10);
  // sprintf(s2,"\n nread:%d s:%s\n",nread,s);
  // write(devfd, s2, strlen(s2));

  // rtems_shell_wait_for_input(
  //   STDIN_FILENO,
  //   20,
  //   notification,
  //   NULL
  // );

  // struct termios term;

  // tcgetattr(STDIN_FILENO, &term);
  // tcsetattr(STDIN_FILENO, TCSANOW, &term);

  // printf("scanf: ");
  // scanf("%s",s);
  // printf("printf: %s\n",s);

  // rtems_status_code status = gpio_set_function(15, GPIO_AF0);


  // status = gpio_set_pull(15, GPIO_PULL_NONE);
  // puts("123");
  // printf("%d", 123);
  // for (int i = 0;i<100;i++) {
  //   printf("hello %d\n", i);

  // }
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