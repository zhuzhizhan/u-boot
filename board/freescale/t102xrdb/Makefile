#
# Copyright 2014 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:      GPL-2.0+
#

ifdef CONFIG_XPL_BUILD
obj-y	+= spl.o
else
obj-y   += t102xrdb.o
obj-$(CONFIG_TARGET_T1024RDB)   += cpld.o
obj-y   += eth_t102xrdb.o
endif
obj-y   += ddr.o
obj-y   += law.o
obj-y   += tlb.o
