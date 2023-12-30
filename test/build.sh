#!/bin/bash
export bsp=raspberrypi4b
export rtemsversion=rtems-aarch64

export PATH=/home/yangn0/devel/$rtemsversion/rtems/6/bin:$PATH

cd /home/yangn0/devel/app/test
rm -rf /home/yangn0/devel/app/test/build

./waf configure --rtems=$HOME/devel/$rtemsversion/rtems/6 --rtems-bsp=aarch64/$bsp
./waf 

mv /home/yangn0/devel/app/test/build/aarch64-rtems6-$bsp/test.exe /home/yangn0/devel/app/test/
rm  -rf /home/yangn0/devel/app/test/build
aarch64-rtems6-objcopy -O binary /home/yangn0/devel/app/test/test.exe kernel8.img
