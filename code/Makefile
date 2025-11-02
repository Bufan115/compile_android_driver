obj-m += memory.o
obj-m += process.o
obj-m += entry.o
# 内核构建目录（GitHub Actions 中会自动设置）
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
