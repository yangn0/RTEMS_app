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
#include <bsp/console.h>
#include <bsp/fatal.h>
#include <bsp/rpi-gpio.h>
#include <bsp/raspberrypi.h>
#include <bsp/spi.h>
#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <bsp.h>
#include <rtems/shell.h>
// SPI

struct Spi0Regs {
    volatile unsigned int cs;
    volatile unsigned int fifo;
    volatile unsigned int clock;
    volatile unsigned int data_length;
    volatile unsigned int ltoh;
    volatile unsigned int dc;
};

#define REGS_SPI0 ((struct Spi0Regs *)(0xFE000000 + 0x00204000))

// CS Register
#define CS_LEN_LONG	(1 << 25)
#define CS_DMA_LEN	(1 << 24)
#define CS_CSPOL2	(1 << 23)
#define CS_CSPOL1	(1 << 22)
#define CS_CSPOL0	(1 << 21)
#define CS_RXF		(1 << 20)
#define CS_RXR		(1 << 19)
#define CS_TXD		(1 << 18)
#define CS_RXD		(1 << 17)
#define CS_DONE		(1 << 16)
#define CS_LEN		(1 << 13)
#define CS_REN		(1 << 12)
#define CS_ADCS		(1 << 11)
#define CS_INTR		(1 << 10)
#define CS_INTD		(1 << 9)
#define CS_DMAEN	(1 << 8)
#define CS_TA		(1 << 7)
#define CS_CSPOL	(1 << 6)
#define CS_CLEAR_RX	(1 << 5)
#define CS_CLEAR_TX	(1 << 4)
#define CS_CPOL__SHIFT	3
#define CS_CPHA__SHIFT	2
#define CS_CS		(1 << 0)
#define CS_CS__SHIFT	0

void spi_send_recv(unsigned char *sbuffer, unsigned char *rbuffer, unsigned int size) {
    REGS_SPI0->data_length = size;
    REGS_SPI0->cs = REGS_SPI0->cs | CS_CLEAR_RX | CS_CLEAR_TX | CS_TA;
    
    unsigned int read_count = 0;
    unsigned int write_count = 0;

    while(read_count < size || write_count < size) {
        while(write_count < size && REGS_SPI0->cs & CS_TXD) {
            if (sbuffer) {
                REGS_SPI0->fifo = *sbuffer++;
            } else {
                REGS_SPI0->fifo = 0;
            }

            write_count++;
        }

        while(read_count < size && REGS_SPI0->cs & CS_RXD) {
            unsigned int data = REGS_SPI0->fifo;

            if (rbuffer) {
                *rbuffer++ = data;
            }

            read_count++;
        }
    }

    while(!(REGS_SPI0->cs & CS_DONE)) {
        while(REGS_SPI0->cs & CS_RXD) {
            unsigned int r = REGS_SPI0->fifo;
        }
    }

    REGS_SPI0->cs = (REGS_SPI0->cs & ~CS_TA);
}

void spi_send(unsigned char *data, unsigned int size) {
    spi_send_recv(data, 0, size);
}

void spi_recv(unsigned char *data, unsigned int size) {
    spi_send_recv(0, data, size);
}

static rtems_status_code init_gpio()
{
    gpio_set_function(7, GPIO_AF0);      //CS1
    gpio_set_pull(8, GPIO_PULL_NONE);    //CS0
    gpio_set_function(9, GPIO_AF0);      //MISO 
    gpio_set_function(10, GPIO_AF0);     //MOSI
    gpio_set_function(11, GPIO_AF0);     //SCLK
}

void reader()
{
  char buf[300];
  int fd = open("/dev/spi0", O_RDWR);
  int rv;
  while (1) {
    rv = read(fd, buf, 3);
    printf("\nRead %d chars\n", rv);
    printf("\nRead data -> %s\n\n", buf);
  }
}

rtems_task Init(
  rtems_task_argument ignored)
{
  unsigned char s[100] = "hello,yangn0";
  int rv;
  char buf[300];
  // init_gpio();
  // while (1) {
  //   puts("spi send");
  //   spi_send(s, 10);
  //   sleep(1);
  // }
  int r = raspberrypi_spi_init("/dev/spi0", (uintptr_t *)BCM2711_SPI0_BASE);

  // DIR *dir;
  // struct dirent *ptr;
  // dir = opendir("/dev");
  // int n = 0;
  // while ((ptr = readdir(dir)) != NULL)
  // {
  //   printf("d_name: %s d_type: %d \n", ptr->d_name, ptr->d_type);
  //   sprintf(s, "/dev/%s", ptr->d_name);
  //   puts(s);
  // }
  // closedir(dir);
  
  int fd = open("/dev/spi0", O_RDWR);

  while (1) {
    rv = write(fd, "This message confirms that RTEMS can send and receive data using the SPI bus to the 23k256 SRAM device!", 104);
    rv = read(fd, buf, 104);
    printf("\nRead %d chars\n", rv);
    printf("\nRead data -> %s\n\n", buf);
    sleep(1);
  }

  close(fd);
  // sleep(1);

  // rtems_id Task_id;
  // rtems_task_create(
  // rtems_build_name('A', 'S', 'E', 'R'),
  //   1, RTEMS_MINIMUM_STACK_SIZE * 2,
  //       RTEMS_DEFAULT_MODES,
  //   RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id);
  // int status = rtems_task_start(Task_id, reader, 1);

  exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
// #define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#define CONFIGURE_FILESYSTEM_IMFS
/* configuration for shell test */
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