#!/bin/bash
export bsp=raspberrypi4b
export rtemsversion=rtems-aarch64

export PATH=$HOME/devel/$rtemsversion/rtems/6/bin:$PATH

cd $HOME/devel/app/test
rm -rf $HOME/devel/app/test/build

./waf configure --rtems=$HOME/devel/$rtemsversion/rtems/6 --rtems-bsp=aarch64/$bsp
./waf 

mv $HOME/devel/app/test/build/aarch64-rtems6-$bsp/test.exe $HOME/devel/app/test/
rm  -rf $HOME/devel/app/test/build
aarch64-rtems6-objcopy -O binary $HOME/devel/app/test/test.exe kernel8.img
