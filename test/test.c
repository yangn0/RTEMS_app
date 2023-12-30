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

rtems_task Init(
    rtems_task_argument ignored)
{
  int n=0;
  // puts("yangn0");
  while (1)
  {
    n++;
    printf("hello %d",n);
    sleep(1);
    printf("hello %d",n);
  }
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#define CONFIGURE_FILESYSTEM_IMFS
// /* configuration for shell test */
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
