#!/bin/bash
export bsp=raspberrypi4b

export PATH=/home/yangn0/devel/rtems-aarch64/rtems/6/bin:$PATH
cd /home/yangn0/devel/rtems-aarch64/app/hello
rm -rf /home/yangn0/devel/rtems-aarch64/app/hello/build
rm /home/yangn0/devel/rtems-aarch64/app/hello/hello.img

./waf configure --rtems=$HOME/devel/rtems-aarch64/rtems/6 --rtems-bsp=aarch64/$bsp
./waf 

aarch64-rtems6-objcopy -O binary /home/yangn0/devel/rtems-aarch64/app/hello/build/aarch64-rtems6-$bsp/hello.exe hello.img

app=hello.exe
qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine virt,gic-version=3 -cpu cortex-a72 -m 4096 -d trace:pl011_baudrate_change -kernel $app