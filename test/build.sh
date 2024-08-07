#!/bin/bash

export arch=aarch64
export bsp=raspberrypi4b
# export bsp=a53_lp64_qemu
# export bsp=a72_lp64_qemu
# export bsp=xilinx_versal_qemu

# export arch=arm
# export bsp=xilinx_zynq_a9_qemu


export PATH=$HOME/RTEMS_devel/rtems/6/bin:$PATH

cd $HOME/RTEMS_app/test
rm -rf $HOME/RTEMS_app/test/build

./waf distclean
./waf configure --rtems=$HOME/RTEMS_devel/rtems/6 --rtems-bsp=$arch/$bsp
./waf 

mv $HOME/RTEMS_app/test/build/$arch-rtems6-$bsp/test.exe $HOME/RTEMS_app/test/
$arch-rtems6-objcopy -O binary $HOME/RTEMS_app/test/test.exe kernel8.img
cp kernel8.img /private/tftpboot/

app=$HOME/RTEMS_app//test/test.exe
# qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine virt,gic-version=3 -cpu cortex-a72 -m 4096 -d trace:pl011_baudrate_change -kernel $app
# qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine virt,gic-version=3 -cpu cortex-a53 -m 4096 -d trace:pl011_baudrate_change -kernel $app
# qemu-system-arm.exe -no-reboot -serial null -serial mon:stdio -net none -nographic -M xilinx-zynq-a9 -m 256M -kernel $app
# qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine xlnx-versal-virt -m 4096 -d trace:pl011_baudrate_change -kernel $app