TARGET_MODULE:=mydrv1

# If we running by kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := driver1.o fileops1.o
	obj-m := $(TARGET_MODULE).o

# If we are running without kernel build system
else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)


all : 
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean

start:
	insmod ./$(TARGET_MODULE).ko

stop:
	rmmod ./$(TARGET_MODULE).ko

restart:
	rmmod ./$(TARGET_MODULE).ko
	insmod ./$(TARGET_MODULE).ko
endif

