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
#include "bsp/console.h"

void write_hello(int fd)
{
  char s1[500] = {"hello RTEMS\n"};
  int ret = write(fd, s1, strlen(s1));
  if (ret <= 0)
  {
    printf("send str error. error no: %d\n", errno);
  }
}

void test_O_NONBLOCK()
{
  puts("test_O_NONBLOCK");

  puts("test open O_RDWR | O_NOCTTY | O_NONBLOCK");
  int fd = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY | O_NONBLOCK);
  printf("fd=%d\n", fd);
  write_hello(fd);
  char buffer[20] = {'\0'};
  puts("read:");
  ssize_t bytes_read = read(fd, buffer, 1);
  printf("bytes_read=%ld ", bytes_read);
  printf("buffer=%s\n", buffer);
  close(fd);

  puts("test open O_RDWR | O_NOCTTY");
  fd = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY);
  printf("fd=%d\n", fd);
  write_hello(fd);
  puts("read:");
  bytes_read = read(fd, buffer, 1);
  printf("bytes_read=%ld ", bytes_read);
  printf("buffer=%s\n", buffer);
  close(fd);
}

void test_posix()
{
  puts("test_posix");
  int fd = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd == -1)
  {
    puts("fd==-1!");
    exit(0);
  }
  write_hello(fd);
  puts("change baud rates to 19200");
  sleep(5);
  puts("Set the baud rates to 19200");
  struct termios options;
  // Get the current options for the port...
  tcgetattr(fd, &options);

  // Set the baud rates to 19200...
  cfsetispeed(&options, B19200);
  cfsetospeed(&options, B19200);

  // Enable the receiver and set local mode...
  options.c_cflag |= (CLOCAL | CREAD);

  // Set the new options for the port...
  tcsetattr(fd, TCSANOW, &options);
  for (int i = 0; i < 10; i++)
  {
    write_hello(fd);
  }
}

void test_input()
{
  puts("test_input");
  char s[500] = {"test string"};
  char c = 'a';
  puts("scanf:");
  scanf("%s", s);
  puts(s);

  while ((c = getchar()) != EOF && c != '\n')
    ;

  puts("getchar:");
  c = getchar();
  printf("%c\n", c);

  while ((c = getchar()) != EOF && c != '\n')
    ;

  puts("gets:");
  gets(s);
  puts(s);
}

void write_all_uart()
{
  puts("test write all UART");

  char s[500] = {"test string\n"};
  int devfd[10] = {-1};

  DIR *dir;
  struct dirent *ptr;
  dir = opendir("/dev");
  int n = 0;
  while ((ptr = readdir(dir)) != NULL)
  {
    printf("d_name: %s d_type: %d \n", ptr->d_name, ptr->d_type);
    sprintf(s, "/dev/%s", ptr->d_name);
    puts(s);
    devfd[n] = open(s, O_RDWR);
    n++;
  }
  closedir(dir);

  while (1)
  {
    for (n = 0; n < 10; n++)
    {
      if (devfd[n] == 0)
        break;
      printf("num:%d,devfd:%d \n", n, devfd[n]);
      write_hello(devfd[n]);
    }
  }
}
static rtems_status_code console_device_init_gpio(
    const bsp_console_device_gpio_config *gpio)
{
  rtems_status_code status = gpio_set_function(gpio->rx, gpio->function);
  if (status != RTEMS_SUCCESSFUL)
    return status;

  status = gpio_set_function(gpio->tx, gpio->function);
  if (status != RTEMS_SUCCESSFUL)
    return status;

  status = gpio_set_pull(gpio->rx, GPIO_PULL_NONE);
  if (status != RTEMS_SUCCESSFUL)
    return status;

  status = gpio_set_pull(gpio->tx, GPIO_PULL_NONE);
  if (status != RTEMS_SUCCESSFUL)
    return status;

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver uart_register(int port)
{
  const bsp_console_device *device = &devices[port];

  rtems_termios_initialize();
  if (device->handler == &mini_uart_handler)
    aux_enable_mini_uart();

  rtems_status_code status = console_device_init_gpio(&device->gpio);
  if (status != RTEMS_SUCCESSFUL)
    bsp_fatal(BSP_FATAL_CONSOLE_REGISTER_DEV_0);

  status = rtems_termios_device_install(device->file, device->handler, NULL,
                                        device->context);
  if (status != RTEMS_SUCCESSFUL)
    bsp_fatal(BSP_FATAL_CONSOLE_INSTALL_0);

  return RTEMS_SUCCESSFUL;
}

void register_all_uarts()
{
  puts("register all UARTs");
  /* register all UART */
  for (int port = 0; port < CONSOLE_DEVICE_COUNT; port++)
  {
    if (port == BSP_CONSOLE_PORT)
      continue;
    if (BSP_CONSOLE_PORT == UART0 && port == UART1)
      continue;
    if (BSP_CONSOLE_PORT == UART1 && port == UART0)
      continue;
    uart_register(port);
  }
}

rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello yangn0");

  register_all_uarts();
  write_all_uart();
  // test_input();
  // test_O_NONBLOCK();
  // test_posix();

  exit(0);
}