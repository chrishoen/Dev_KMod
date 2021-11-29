KDIR := $(HOME)/kernel/rpi/linux
CROSS_DIR:=$(HOME)/kernel/rpi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-
PWD:=$(shell pwd)

TARGET_MODULE:=mydrv3
$(TARGET_MODULE)-objs := driver3.o proc3.o gpio3.o
obj-m := $(TARGET_MODULE).o

all:
	make -C $(KDIR) \
		ARCH=arm CROSS_COMPILE=$(CROSS_DIR) \
		M=$(PWD) modules
	scp $(TARGET_MODULE).ko pi@updev1:/opt/prime/stage

clean:
	make -C $(KDIR) \
		ARCH=arm CROSS_COMPILE=$(CROSS_DIR) \
		SUBDIRS=$(PWD) clean

install:
	scp $(TARGET_MODULE).ko pi@updev1:/opt/prime/stage


