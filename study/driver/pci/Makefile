#
# NVM Express Compliance Suite
# Copyright (c) 2011, Intel Corporation.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#

# Modify the Makefile to point to Linux build tree.
PWD := $(shell pwd)
KERNELDIR ?=/lib/modules/$(shell uname -r)/build/

obj-m := snvme.o


all:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

