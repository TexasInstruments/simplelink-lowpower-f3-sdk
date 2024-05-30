#------------------------------------------------------------------------------
#
#    Module        : DDK-130_bsd
#    Version       : 4.1.1
#    Configuration : DDK_EIP130_BSD
#
#    Date          : 2023-Mar-10
#
# Copyright (c) 2014-2023 by Rambus, Inc. and/or its subsidiaries.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#------------------------------------------------------------------------------

##############################################################################
##  Makefile for the Linux Kernel Support Driver (UMDevXS only)
##  How to use :
##  i)    $ make
##  ii)   For cleaning : $ make clean
##############################################################################

#### User Edit Section : Variables to be set by the User
##   User may not need to modify anything other than this section

  ##-->: { Here follow the variables to be set by the User

	# Set ARCH according to the target architecture
	# supported values: x86, arm, arm64 and powerpc
	#ARCH:=x86
	#ARCH:=arm
	#ARCH:=arm64
	#ARCH:=powerpc

	# Set KERNEL_DIR to the path to the Kernel Source directory for which you
	# wish to compile.
	#KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

	# Set HOST_HW_PLATFORM
	# supported values: FPGA_V2M_Juno, FPGA_Virtex6_PCI and FPGA_Zynq_ZC702
	#HOST_HW_PLATFORM:=FPGA_V2M_Juno
	#HOST_HW_PLATFORM:=FPGA_Virtex6_PCI
	#HOST_HW_PLATFORM:=FPGA_Zynq_ZC702

  ##<--:  } End of User Edit Section

#### User should not need to modify anything below this line
##############################################################################

SYSTEMTEST_CONFIGURATION?=

## Make sure that ARCH, KERNEL_DIR, HOST_HW_PLATFORM and CROSS_COMPILE are set

ifndef KERNEL_DIR
  ERROR_KERNEL_DIR_NOT_SET:=YES
endif

ifeq (${DDK_HOST64},y)
  EXTRA_CFLAGS += -DUMDEVXS_64BIT_HOST
endif

ifeq (${DDK_DEVICE64},y)
  EXTRA_CFLAGS += -DUMDEVXS_64BIT_DEVICE
endif

ifeq (${HOST_HW_PLATFORM},FPGA_V2M_Juno)
  EIP130_BUS_TYPE:=OF
else ifeq (${HOST_HW_PLATFORM},FPGA_Zynq_ZC702)
  EIP130_BUS_TYPE:=OF
else ifeq (${HOST_HW_PLATFORM},FPGA_Zynq_ZC706)
  EIP130_BUS_TYPE:=OF
else ifeq (${HOST_HW_PLATFORM},FPGA_Virtex6_PCI)
  EIP130_BUS_TYPE:=PCI
else ifeq (${HOST_HW_PLATFORM},FPGA_Virtex7_PCI)
  EIP130_BUS_TYPE:=PCI
else
  ERROR_HOST_HW_PLATFORM_NOT_SET:=YES
endif

ifeq (${ARCH},x86)
  HOST_COMPILER:=gcc
  COMPILER:=gcc
  AR:=ar
  # cs_*.h files respond to ARCH_X86
  EXTRA_CFLAGS += -DARCH_X86
else ifeq (${ARCH},powerpc)
  ifndef CROSS_COMPILE
    ERROR_CROSS_COMPILE_NOT_SET:=YES
  endif
  HOST_COMPILER:=gcc
  COMPILER:=${CROSS_COMPILE}gcc
  AR:=${CROSS_COMPILE}ar
  EXTRA_CFLAGS += -DARCH_POWERPC
else ifeq (${ARCH},arm)
  ifndef CROSS_COMPILE
    ERROR_CROSS_COMPILE_NOT_SET:=YES
  endif
  HOST_COMPILER:=gcc
  COMPILER:=${CROSS_COMPILE}gcc
  AR:=${CROSS_COMPILE}ar
  EXTRA_CFLAGS += -DARCH_ARM
else ifeq (${ARCH},arm64)
  ifndef CROSS_COMPILE
    ERROR_CROSS_COMPILE_NOT_SET:=YES
  endif
  HOST_COMPILER:=gcc
  COMPILER:=${CROSS_COMPILE}gcc
  AR:=${CROSS_COMPILE}ar
  EXTRA_CFLAGS += -DARCH_ARM64
else
  ERROR_ARCH_NOT_SET:=YES
endif

ifneq (,$(findstring s,${MAKEFLAGS}))
  AR_VERBOSE:=
else
  AR_VERBOSE:=v
endif

ifeq ($(KERNELRELEASE),)
    PWD:=$(shell pwd)
endif

ifeq ($(DDK_PWD_OVERRIDE),)
   DDK_PWD_OVERRIDE := `pwd`
else
   override PWD := $(DDK_PWD_OVERRIDE)
endif

PATH_BASE=$(PWD)/../../../..
UMDEVXS_BASE=../../../../Integration

# Include Paths
INCLUDE_ALL:= \
  $(SYSTEMTEST_CONFIGURATION) \
  -I$(PWD) \
  -I$(PWD)/$(HOST_HW_PLATFORM) \
  -I$(PATH_BASE)/Config \
  -I$(PATH_BASE)/Config/$(HOST_HW_PLATFORM) \
  -I$(PATH_BASE)/Kit/DriverFramework/Basic_Defs_API/incl \
  -I$(PATH_BASE)/Kit/DriverFramework/CLib_Abstraction_API/incl \
  -I$(PATH_BASE)/Kit/Log/incl \
  -I$(PATH_BASE)/Kit/Log/src/printk \
  -I$(PATH_BASE)/Integration/DriverFramework/src/LKM \
  -I$(PATH_BASE)/Integration/DriverFramework/src/LKM/$(EIP130_BUS_TYPE) \
  -I$(PATH_BASE)/Integration/UMDevXS/KernelPart/incl \
  -I$(PATH_BASE)/Integration/UMDevXS/KernelPart/src

# Compiler Flags
USER_FLAGS_WARN=-Wall

ifeq (${ARCH},x86)
USER_FLAGS_WARN += -Wextra -Wvla -Wlogical-op \
                   -Wstrict-prototypes -Wmissing-prototypes \
                   -Wundef -Wmissing-declarations -Wdouble-promotion \
                   -Wjump-misses-init -Wwrite-strings -Wformat=2 \
                   -Wcast-align -Wno-unused-parameter -Wno-discarded-qualifiers

USER_FLAGS_WARN += ${WARNING_FLAGS}
endif

# supress kernel header files issues
# -Wshadow -Wnested-externs -Wcast-qual -Wpointer-arith -Wconversion -Wsign-conversion -Wswitch-enum

#DEBUG_FLAGS=-g

EXTRA_CFLAGS += \
	$(INCLUDE_ALL) \
	$(USER_FLAGS_WARN) \
	$(DEBUG_FLAGS)

.PHONY = clean

general_k-objs:= \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_lkm.o \
    $(UMDEVXS_BASE)/DriverFramework/src/LKM/$(EIP130_BUS_TYPE)/lkm.o \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_chrdev.o \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_device.o \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_smbuf.o \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_bufadmin.o \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_interrupt.o \
    $(UMDEVXS_BASE)/../Kit/Log/src/log.o

ifeq (${EIP130_BUS_TYPE},OF)
  obj-m:=umdevxs_k.o
  umdevxs_k-objs:= \
    $(general_k-objs) \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_ofdev.o
else ifeq (${EIP130_BUS_TYPE},PCI)
  obj-m:=umpci_k.o
  umpci_k-objs:= \
    $(general_k-objs) \
    $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/umdevxs_pcidev.o
endif

default: build

build:
	@if [ "$(ERROR_ARCH_NOT_SET)" = "YES" ]; then \
	  echo "ARCH not set or unsupported"; \
	  exit 1; \
	fi
	@if [ "$(ERROR_KERNEL_DIR_NOT_SET)" = "YES" ]; then \
	  echo "KERNEL_DIR not set"; \
	  exit 1; \
	fi
	@if [ "$(ERROR_CROSS_COMPILE_NOT_SET)" = "YES" ]; then \
	  echo "CROSS_COMPILE not set"; \
	  exit 1; \
	fi
	@if [ "$(ERROR_HOST_HW_PLATFORM_NOT_SET)" = "YES" ]; then \
	  echo "HOST_HW_PLATFORM not set"; \
	  exit 1; \
	fi
#	@echo "EXTRA_CFLAGS: $(EXTRA_CFLAGS)";
	$(MAKE) DDK_PWD_OVERRIDE=$(DDK_PWD_OVERRIDE) $(EXTRA_KBUILD_FLAGS) ARCH=$(ARCH) CROSS_COMPILE=$(strip $(CROSS_COMPILE)) -C $(KERNEL_DIR) SUBDIRS=$(PWD) M=$(PWD) modules

clean:	clean_module

clean_module:
	-rm -rf .tmp_versions *.symvers *.order *.markers *.o .*.cmd .*.flags *.mod.c *.ko
	-rm -f $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/*.o $(UMDEVXS_BASE)/UMDevXS/KernelPart/src/.*.cmd
	-rm -f $(UMDEVXS_BASE)/DriverFramework/src/LKM/$(EIP130_BUS_TYPE)/*.o $(UMDEVXS_BASE)/DriverFramework/src/LKM/$(EIP130_BUS_TYPE)/.*.cmd
	-rm -f $(UMDEVXS_BASE)/../Kit/Log/src/*.o $(UMDEVXS_BASE)/../Kit/Log/src/.*.cmd

# end of file
