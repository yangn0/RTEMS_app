#include <rtems.h>
#include <stdlib.h>
#include <stdio.h>
#include <bsp/mbox.h>
#include <bsp/mbox/property/message.h>
#include <bsp/mbox/property/tags/hardware.h>
#include <bsp/mbox/property/tags.h>
#include <bsp/mbox/property.h>

rtems_task Init(
    rtems_task_argument ignored)
{
  puts("hello yangn0");
  mbox_property_tag_metadata tag_metadata[] = {
      HARDWARE_GET_BOARD_MAC_ADDRESS_TAG,
  };
  mbox_property_message message;
  mbox_property_message_init(&message, 0, tag_metadata, RTEMS_ARRAY_SIZE(tag_metadata));
  // int sent_mail = mbox_property_compose_mail(&message);
  // mbox_write(sent_mail);
  // const mbox_mail received_mail = mbox_read();

  mbox_property_send_message(&message);
  // printf("mbox_property_send_message: %d\n",mbox_property_send_message(&message));
  // mbox_property_tag *clocks = (mbox_property_tag *)message->buffer;
  // hardware_get_board_mac_address_tag_buffer *buf =
  //     (hardware_get_board_mac_address_tag_buffer *)clocks->buffer;
  // for (unsigned int i = 0;
  //      i < RTEMS_ARRAY_SIZE(buf->response.board_mac_address); i++)
  //   printf("mac[%u] = %hu\n", i, buf->response.board_mac_address[i]);


  puts("end");
  exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
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