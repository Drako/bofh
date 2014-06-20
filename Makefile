obj-m := bofh.o
bofh-objs := bofh_main.o bofh_excuses.o

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

