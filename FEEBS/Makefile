KERNEL_VER = $(shell uname -r)
PWD = $(shell pwd)
# the file to compile
obj-m += soyo_feebs_c2b.o soyo_feebs_b2c.o soyo_feebs_base.o soyo_feebs_mon.o soyo_feebs_dev.o
# specify flags for the module compilation
EXTRA_CFLAGS = -g -O0
build: kernel_modules
kernel_modules:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	insmod soyo_feebs_c2b.ko
	insmod soyo_feebs_b2c.ko
	insmod soyo_feebs_base.ko
	insmod soyo_feebs_mon.ko
	insmod soyo_feebs_dev.ko
#	/usr/bin/sh /FEEBS/InitDat.sh
#	cat  /proc/SOYO_FEEBS_BASE 
clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean
	rmmod soyo_feebs_c2b 
	rmmod soyo_feebs_b2c
	rmmod soyo_feebs_base
	rmmod soyo_feebs_mon
	rmmod soyo_feebs_dev
