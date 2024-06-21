#!/bin/bash
export bsp=raspberrypi4b
export rtemsversion=rtems-aarch64

# export PATH=$HOME/devel/rtems-aarch64-utk/rtems/6/bin:$PATH
export PATH=$HOME/devel/$rtemsversion/rtems/6/bin:$PATH
export RTEMS_MAKEFILE_PATH=$HOME/devel/$rtemsversion/rtems/6/aarch64-rtems6/$bsp
export PROJECT_ROOT=$HOME/devel/$rtemsversion/src/rtems

cd $HOME/devel/app/test
make clean
make 

mv $HOME/devel/app/test/o-optimize/test.exe $HOME/devel/app/test/

rm -rf $HOME/devel/app/test/o-optimize/

aarch64-rtems6-objcopy -O binary $HOME/devel/app/test/test.exe kernel8.img

cp kernel8.img /mnt/c/Users/79230/Desktop/tftp/