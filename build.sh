#!/bin/bash
export rtems_version=6
export arch=aarch64
# export bsp=raspberrypi4b
# export bsp=a53_lp64_qemu
# export bsp=a72_lp64_qemu
# export bsp=xilinx_versal_qemu
export bsp=zynqmp_qemu

# export arch=arm
# export bsp=xilinx_zynq_a9_qemu


export PATH=$HOME/RTEMS_devel/rtems/$rtems_version/bin:$PATH

cd $HOME/RTEMS_app/
rm -rf $HOME/RTEMS_app/build

./waf distclean
./waf configure --rtems=$HOME/RTEMS_devel/rtems/$rtems_version --rtems-bsp=$arch/$bsp
./waf 

mv $HOME/RTEMS_app/build/$arch-rtems$rtems_version-$bsp/test.exe $HOME/RTEMS_app/
# $arch-rtems6-objcopy -O binary $HOME/RTEMS_app/test.exe kernel8.img
# cp kernel8.img /mnt/c/Users/79230/Desktop/tftp/

# qemu-system-aarch64 -M raspi4b -serial mon:stdio -nographic -kernel /mnt/c/Users/79230/Desktop/tftp/kernel8.img

app=$HOME/RTEMS_app/test.exe
# qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine virt,gic-version=3 -cpu cortex-a72 -m 4096 -d trace:pl011_baudrate_change -kernel $app
# qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine virt,gic-version=3 -cpu cortex-a53 -m 4096 -d trace:pl011_baudrate_change -kernel $app
# qemu-system-arm.exe -S -s -no-reboot -net none -nographic -M realview-pbx-a9 -m 256M -d trace:pl011_baudrate_change -kernel $app
# qemu-system-aarch64.exe -no-reboot -nographic -serial mon:stdio -machine xlnx-versal-virt -m 4096 -d trace:pl011_baudrate_change -kernel $app

# qemu-system-arm.exe -no-reboot -serial null -serial mon:stdio -net none -nographic -M xilinx-zynq-a9 -m 256M -kernel $app

if [ "$bsp" = xilinx_zynq_a9_qemu ]; then

# sudo ip tuntap add qtap mode tap user $(whoami)
# sudo ip link set dev qtap up
# sudo ip addr add 169.254.1.1/16 dev qtap

qemu-system-arm -serial null -serial mon:stdio -nographic \
  -M xilinx-zynq-a9 -m 256M \
  -net nic,model=cadence_gem \
  -net tap,ifname=qtap,script=no,downscript=no \
  -kernel $app
fi

if [ "$bsp" = zynqmp_qemu ]; then
# sudo ip tuntap add qtap mode tap user $(whoami)
# sudo ip link set dev qtap up
# sudo ip addr add 169.254.1.1/16 dev qtap
qemu-system-aarch64 -no-reboot -nographic  \
    -serial mon:stdio -machine xlnx-zcu102 -m 4096 \
    -net nic,model=cadence_gem \
    -net tap,ifname=qtap,script=no,downscript=no \
    -kernel $app
fi