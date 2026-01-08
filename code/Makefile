# Kernel driver Makefile for Android 15 (6.6.66)
CONFIG_MODULE_SIG := n
CONFIG_MODULE_SIG_ALL := n
CONFIG_MODULE_SIG_SHA256 := n

obj-$(CONFIG_KERNEL_HACK) += khack.o

khack-y := \
    entry.o \
    memory.o \
    process.o

ccflags-y := -DDEBUG

# 禁用模块签名
CONFIG_MODULE_SIG := n