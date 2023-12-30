#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/test.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = test_uart.c init_uart.c
# CSRCS = test_shell.c init_shell.c
# CSRCS = test.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)


include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)


all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)