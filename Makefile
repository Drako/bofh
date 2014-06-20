kernel_version := $(shell uname -r)

obj-m := bofh.o
bofh-objs := bofh_main.o bofh_excuses.o

modules:
	make -C /lib/modules/$(kernel_version)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(kernel_version)/build M=$(PWD) clean

