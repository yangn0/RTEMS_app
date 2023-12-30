#include <stdlib.h>
#include <stdio.h>
#include <bsp.h>
#include <rtems/shell.h>
static rtems_task user_task(rtems_task_argument ignored)
{
    printf("In User tASK\n");
    printf("starting shell\n");

    rtems_shell_init(
        "SHLL",                       /* task_name */
        RTEMS_MINIMUM_STACK_SIZE * 4, /* task_stacksize */
        100,                          /* task_priority */
        "/dev/ttyS1",               /* devname */
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
    for (i = 0; i < 3; i++)
    {
        printf("\n\n*** HELLO WORLD Etual ***\n");
        sleep(1);
    }

    Task_name = rtems_build_name('U', 'S', 'E', 'R');

    status = rtems_task_create(
        Task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
        RTEMS_DEFAULT_MODES,
        RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id);

    status = rtems_task_start(Task_id, user_task, 1);
    status = rtems_task_delete(RTEMS_SELF);
}


