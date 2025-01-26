#
# Hello world Waf script
#
from __future__ import print_function

rtems_version = "6"

try:
    import rtems_waf.rtems as rtems
except:
    print('error: no rtems_waf git submodule')
    import sys
    sys.exit(1)

def init(ctx):
    rtems.init(ctx, version = rtems_version, long_commands = True)

def bsp_configure(conf, arch_bsp):
    # Add BSP specific configuration checks
    pass

def options(opt):
    rtems.options(opt)

def configure(conf):
    rtems.configure(conf, bsp_configure = bsp_configure)

def build(bld):
    rtems.build(bld)

    bld(features = 'c cprogram',
        target = 'test.exe',
        cflags = '-g -O0',
        includes = [
                    '/home/yangn0/RTEMS_devel/src/rtems-libbsd/testsuite/include/',
                    '/home/yangn0/RTEMS_devel/src/rtems-libbsd/testsuite/include/rtems/bsd/test/',
                    # '/home/yangn0/RTEMS_devel/src/rtems-lwip/rtemslwip/test'
                    ],
        # libpath = '/home/yangn0/RTEMS_devel/rtems/6/aarch64-rtems6/raspberrypi4b/lib/',
        # defines      = ["Z_PREFIX","LWIP_DEBUG"],
        # lib = ['curl','bsd','rtemstest','m','z','tftpfs', 'ftpd', 'telnetd'],
        # lib=['rtemscpu', 'rtemsbsp', 'telnetd','bsd','rtemstest','ftpd','m','z'],
        source = [
            # './udp_echo_server/rtems_config.c','./udp_echo_server/udp_echo_server.c'
            # 'test_uart.c'
            # 'test_shell.c'
            # 'test_fdt.c'
            # 'test_libbsd.c',
            # 'test_lwip.c',
            # "test_select.c"
            # "test_unix.c",
            "test_fifo.c"
                  ])
