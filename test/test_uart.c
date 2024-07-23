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

// void write_hello(int fd)
// {
//   char s1[500] = {"hello RTEMS\n"};
//   int ret = write(fd, s1, strlen(s1));
//   if (ret <= 0)
//   {
//     printf("send str error. error no: %d\n", errno);
//   }
// }

// void test_O_NONBLOCK()
// {
//   puts("test_O_NONBLOCK");

//   puts("test open O_RDWR | O_NOCTTY | O_NONBLOCK");
//   int fd = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY | O_NONBLOCK);
//   printf("fd=%d\n", fd);
//   write_hello(fd);
//   char buffer[20] = {'\0'};
//   puts("read:");
//   ssize_t bytes_read = read(fd, buffer, 1);
//   printf("bytes_read=%ld ", bytes_read);
//   printf("buffer=%s\n", buffer);
//   close(fd);

//   puts("test open O_RDWR | O_NOCTTY");
//   fd = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY);
//   printf("fd=%d\n", fd);
//   write_hello(fd);
//   puts("read:");
//   bytes_read = read(fd, buffer, 1);
//   printf("bytes_read=%ld ", bytes_read);
//   printf("buffer=%s\n", buffer);
//   close(fd);
// }

// void test_posix()
// {
//   puts("test_posix");
//   int fd = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY | O_NONBLOCK);
//   if (fd == -1)
//   {
//     puts("fd==-1!");
//     exit(0);
//   }
//   write_hello(fd);
//   puts("change baud rates to 19200");
//   sleep(5);
//   puts("Set the baud rates to 19200");
//   struct termios options;
//   // Get the current options for the port...
//   tcgetattr(fd, &options);

//   // Set the baud rates to 19200...
//   cfsetispeed(&options, B19200);
//   cfsetospeed(&options, B19200);

//   // Enable the receiver and set local mode...
//   options.c_cflag |= (CLOCAL | CREAD);

//   // Set the new options for the port...
//   tcsetattr(fd, TCSANOW, &options);
//   for (int i = 0; i < 10; i++)
//   {
//     write_hello(fd);
//   }
// }

// void test_input()
// {
//   puts("test_input");
//   char s[10]={0};
//   char c = 'a';
//   puts("scanf:");
//   scanf("%s", s);
//   puts(s);

//   while ((c = getchar()) != EOF && c != '\n')
//     ;

//   puts("getchar:");
//   c = getchar();
//   printf("%c\n", c);

//   while ((c = getchar()) != EOF && c != '\n')
//     ;

//   puts("gets:");
//   gets(s);
//   puts(s);
// }

// void write_all_uart()
// {
//   puts("test write all UART");

//   char s[500] = {"test string\n"};
//   int devfd[10] = {-1};

//   DIR *dir;
//   struct dirent *ptr;
//   dir = opendir("/dev");
//   int n = 0;
//   while ((ptr = readdir(dir)) != NULL)
//   {
//     printf("d_name: %s d_type: %d \n", ptr->d_name, ptr->d_type);
//     sprintf(s, "/dev/%s", ptr->d_name);
//     puts(s);
//     devfd[n] = open(s, O_RDWR);
//     n++;
//   }
//   closedir(dir);

//   while (1)
//   {
//     for (n = 0; n < 10; n++)
//     {
//       if (devfd[n] == 0)
//         break;
//       printf("num:%d,devfd:%d \n", n, devfd[n]);
//       write_hello(devfd[n]);
//       sleep(1);
//     }
//   }
// }

// rtems_device_driver uart_register(int port)
// {
//   extern const bsp_console_device devices[];
//   const bsp_console_device *device = &devices[port];

//   rtems_termios_initialize();
//   if (device->handler == &mini_uart_handler)
//     aux_enable_mini_uart();

//   rtems_status_code status = console_device_init_gpio(&device->gpio);
//   if (status != RTEMS_SUCCESSFUL)
//     bsp_fatal(BSP_FATAL_CONSOLE_REGISTER_DEV_0);

//   status = rtems_termios_device_install(device->file, device->handler, NULL,
//                                         device->context);
//   if (status != RTEMS_SUCCESSFUL)
//     bsp_fatal(BSP_FATAL_CONSOLE_INSTALL_0);

//   return RTEMS_SUCCESSFUL;
// }

// void register_all_uarts()
// {
//   puts("register all UARTs");
//   /* register all UART */
//   for (int port = 0; port < CONSOLE_DEVICE_COUNT; port++)
//   {
//     if (port == BSP_CONSOLE_PORT)
//       continue;
//     if (BSP_CONSOLE_PORT == UART0 && port == UART1)
//       continue;
//     if (BSP_CONSOLE_PORT == UART1 && port == UART0)
//       continue;
//     uart_register(port);
//   }
// }
// #define FR(base)                REG(base + 0x18)
// #define FR_TXFF                 BSP_BIT32(5)
// static inline bool arm_pl011_is_txfifo_full(const uintptr_t regs_base) {
//     return (FR(regs_base) & FR_TXFF) != 0;
// }

// #define IMSC(base)              REG(base + 0x38)
// static inline void enable_irq(const uintptr_t regs_base,
//                               const uint32_t irq_mask) {
//     IMSC(regs_base) |= irq_mask;
// }

// static inline void disable_irq(uintptr_t regs_base, uint32_t irq_mask) {
//     IMSC(regs_base) &= ~irq_mask;
// }

rtems_task Init(
    rtems_task_argument ignored)
{
  // sleep(1);
  puts("hello");
  // sleep(1);
  // uart_register(UART2);
  // int devfd = open("/dev/ttyAMA1", O_RDWR);

  

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