#!/bin/bash

export bsp=raspberrypi4b
# export bsd=xilinx_versal_qemu
# export bsd=xilinx_zynqmp_lp64_qemu

cd /home/yangn0/devel/rtems-aarch64/app/rtems-btstack
export PATH=/home/yangn0/devel/rtems-aarch64/rtems/6/bin:$PATH
export RTEMS_MAKEFILE_PATH=/home/yangn0/devel/rtems-aarch64/rtems/6/aarch64-rtems6/$bsp
export PROJECT_ROOT=/home/yangn0/devel/rtems-aarch64/src/rtems
make clean


make 

aarch64-rtems6-objcopy -O binary /home/yangn0/devel/rtems-aarch64/app/rtems-btstack/o-optimize/app.exe app.img
mv /home/yangn0/devel/rtems-aarch64/app/rtems-btstack/o-optimize/app.exe ./