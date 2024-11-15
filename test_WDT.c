#include <rtems.h>
#include <stdio.h>
#include <unistd.h>
#include <bsp/watchdog.h>

rtems_task Init(
    rtems_task_argument ignored)
{
  int timeout;
  puts("hello");

  raspberrypi_watchdog_init();

  raspberrypi_watchdog_start(15999);
  printf("%d\n", raspberrypi_watchdog_get_remaining_time());
  raspberrypi_watchdog_stop();
  sleep(3);
  printf("%d\n", raspberrypi_watchdog_get_remaining_time());

  sleep(1);
  raspberrypi_watchdog_reload();
  while(1)
  {
    printf("%d\n", raspberrypi_watchdog_get_remaining_time());
  }
  
  puts("end");

  // raspberrypi_watchdog_system_reset();

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