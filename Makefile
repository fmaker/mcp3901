################################################################################
# Copyright 2011 Frank Maker <frank.maker@gmail.com>
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the license, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
################################################################################

# Invoked from kernel build system
ifneq ($(KERNELRELEASE),)
	obj-m := mcp3901.o
# Otherwise
else
	#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	KERNELDIR ?= ~/opt/aosp/kernel/omap/
	PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

.PHONY: clean

clean:
	rm -rf *.o *.~ *.ko *.mod.c *.order *.symvers

