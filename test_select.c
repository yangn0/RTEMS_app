#include <rtems.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <machine/rtems-bsd-commands.h>

#define TEST_NAME "LIBBSD SELECT"

void
delay(int us)
{
  struct timeval tv;

  tv.tv_sec = 5;
  tv.tv_usec = us;
  (void) select(1, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}


static void
test_main(void)
{
  puts("hello");

  // delay(10000);
  fd = open();

  puts("end");

  rtems_shutdown_executive(0);
}

#include <rtems/bsd/test/default-init.h>