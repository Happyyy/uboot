#
# U-boot - Makefile
#
# Copyright (c) 2013 Analog Device Inc.
#
# (C) Copyright 2000-2012
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y	:= lowlevel_init.o
obj-y	+= soc.o timer.o gpio.o
obj-y	+= clocks.o
extra-y := init
extra-y += init.elf
ifeq ($(CONFIG_SC58X_CHAIN_BOOT),y)
extra-y += init.ldr
endif
extra-y += init.o
extra-y += init_start.o
extra-y += init_main.o
extra-y += dmcinit.o

quiet_cmd_link_init = LD      $@
      cmd_link_init = $(LD) -Ttext=0x20080000 -nostdlib $(LDFLAGS) -o $@ $^

$(obj)/init: $(obj)/init_start.o $(obj)/init.o $(obj)/dmcinit.o
	$(call if_changed,link_init)

$(obj)/init.elf: $(obj)/init_main.o $(obj)/init.o $(obj)/dmcinit.o
	$(call if_changed,link_init)

ifeq ($(CONFIG_SC58X_CHAIN_BOOT),y)
quiet_cmd_ldr_init = 
      cmd_ldr_init = $(LDR) -T $(CONFIG_CPU) -c $@ $^  --bcode=1 --use-vmas

$(obj)/init.ldr: $(obj)/init
	$(call if_changed,ldr_init)
endif
