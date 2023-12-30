#include <sys/param.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/filio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include <machine/rtems-bsd-commands.h>

#include <rtems/libcsupport.h>
#include <rtems.h>

#include <termios.h>
#include <rtems/error.h>
#include <rtems/bsd/bsd.h>

#include <pthread.h>
#include <math.h>

#include <dirent.h>
void *worker();

#define TEST_NAME "btstack"

#include <rtems/shell.h>
rtems_task user_task(rtems_task_argument ignored)
{
    printf(" =========================\n");
    printf(" starting shell\n");
    printf(" =========================\n");
    rtems_shell_init(
        "SHLL",                       /* task name */
        RTEMS_MINIMUM_STACK_SIZE * 4, /* task stack size */
        100,                          /* task priority */
        "/dev/ttyAMA3",               /* device name */
        false,                        /* run forever */
        true,                         /* wait for shell to terminate */
        NULL                          /* login check function,
                           use NULL to disable a login check */
    );
}

void start_shell(rtems_task_argument ignored)
{
    rtems_id Task_id;
    rtems_name Task_name;
    Task_name = rtems_build_name('B', 'T', 's', 't');
    rtems_task_create(
        Task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
        RTEMS_DEFAULT_MODES,
        RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id);
    rtems_task_start(Task_id, user_task, 1);
}

void print_dir()
{
    struct stat s;
    DIR *dir;
    struct dirent *ptr;
    char s1[500] = {0};
    int fd = open("/dev/ttyAMA3", O_RDWR | O_NOCTTY);
    while (1)
    {
        dir = opendir("/");
        while ((ptr = readdir(dir)) != NULL)
        {
            stat(ptr->d_name, &s);
            sprintf(s1, "d_name: %s d_type: %d size = %ld \n", ptr->d_name, ptr->d_type, s.st_size);
            write(fd, s1, strlen(s1));
        }
        closedir(dir);
        sleep(1);
    }
}

void start_print_dir()
{
    rtems_id Task_id;
    rtems_name Task_name;
    Task_name = rtems_build_name('P', 'R', 'I', 'T');
    rtems_task_create(
        Task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
        RTEMS_DEFAULT_MODES,
        RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id);
    rtems_task_start(Task_id, print_dir, 1);
}


static void
test_main(void)
{
    // start_shell();
    start_print_dir();
    printf("hello btstack\n");
    worker();
    puts("end");
}

// #include <default-init.h>
#include <default-termios-init.h>
