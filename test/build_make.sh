#!/bin/bash
export bsp=raspberrypi4b
export rtemsversion=rtems-aarch64

# export PATH=/home/yangn0/devel/rtems-aarch64-utk/rtems/6/bin:$PATH
export PATH=/home/yangn0/devel/$rtemsversion/rtems/6/bin:$PATH
export RTEMS_MAKEFILE_PATH=/home/yangn0/devel/$rtemsversion/rtems/6/aarch64-rtems6/$bsp
export PROJECT_ROOT=/home/yangn0/devel/$rtemsversion/src/rtems

cd /home/yangn0/devel/app/test
make clean
make 

mv /home/yangn0/devel/app/test/o-optimize/test.exe /home/yangn0/devel/app/test/

rm -rf /home/yangn0/devel/app/test/o-optimize/

aarch64-rtems6-objcopy -O binary /home/yangn0/devel/app/test/test.exe kernel8.img
