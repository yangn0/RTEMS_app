#include <rtems.h>
#include <stdio.h>
#include <unistd.h>

#include <libfdt.h>
#include <bsp/fdt.h>

rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello");

  const void *fdt;
  int node;
  int len;
  const uint32_t *val;

  fdt = bsp_fdt_get();
  node = fdt_node_offset_by_compatible(fdt, -1, "brcm,bcm2835-sdhci");

  printf("%d\n", node);

  val = fdt_getprop(fdt, node, "reg", &len);

  printf("%lX %lX\n", fdt32_to_cpu(val[0]), len);


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