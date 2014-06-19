obj-m := bofh.o
bofh-objs := bofh_main.o bofh_excuses.o

modules:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean

