#include <stdlib.h>
#include <stdio.h>
#include <bsp.h>
#include <rtems/shell.h>
static rtems_task user_task(rtems_task_argument ignored)
{
    printf("starting shell\n");

    rtems_shell_init(
        "SHLL",                       /* task_name */
        RTEMS_MINIMUM_STACK_SIZE * 4, /* task_stacksize */
        100,                          /* task_priority */
        "/dev/console",               /* devname */
        false,                        /* forever */
        true,                         /* wait */
        NULL                          /* login */
    );
    printf("Initialized shell.\n");

}

rtems_task Init(rtems_task_argument ignored)
{
    rtems_name Task_name;
    rtems_id Task_id;
    rtems_status_code status;
    int i;

    Task_name = rtems_build_name('U', 'S', 'E', 'R');

    status = rtems_task_create(
        Task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
        RTEMS_DEFAULT_MODES,
        RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id);

    status = rtems_task_start(Task_id, user_task, 1);
    status = rtems_task_delete(RTEMS_SELF);
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

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#include <rtems/shellconfig.h>
