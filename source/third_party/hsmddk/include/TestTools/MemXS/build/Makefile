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
##  Makefile for MemXS utility
##  How to use :
##  i)    $ make
##  ii)   For cleaning : $ make clean
##############################################################################

#### User Edit Section : Variables to be set by the User
##   User may not need to modify anything other than this section

  ##-->: { Here follow the variables to be set by the User

	# Set KERNEL_DIR to the path to the Kernel Source directory for which you
	# wish to compile.
	#KERNEL_DIR:=

	# Set ARCH according to the target architecture
	# supported values: x86, arm, arm64 and powerpc
	#ARCH:=x86
	#ARCH:=arm
	#ARCH:=arm64
	#ARCH:=powerpc

	# Set PATH_DF_LIB to the Driver Framework library build directory where
	# the $(LIB_DF_NAME) library file is located. The same directory must
	# contain the configuration files. If not set then a default
	# build location will be used.

	# Set CROSS_COMPILE to the path to the cross compiler binutils
	# concatenated with the prefix they have before normal binutils name
	# For example :
	# If we have cross compiler for Power-PC in /opt/eldk/target/usr/bin/
	# by the file name ppc_4xxFP-gcc, we set it as below :
	#CROSS_COMPILE:=/opt/eldk/target/usr/bin/

	# Set CROSS_LIB_PATH to the path to the cross compiler libraries
	#CROSS_LIB_PATH:=/opt/eldk/ppc_4xxFP/lib

	# Path to DDK-internal components
	PATH_KIT:=../../../Kit
        PATH_CONFIG:= ../../../Config

	# Driver Framework library name
	LIB_DF_NAME:=df

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

ifndef HOST_HW_PLATFORM
    ERROR_HOST_HW_PLATFORM_NOT_SET:=YES
endif

ifeq (${ARCH},x86)
    HOST_COMPILER:=gcc
    COMPILER:=gcc
    EXTRA_CFLAGS += -DARCH_X86
    LIB_PATH=/usr/lib
    STATICFLAGS =
else ifeq (${ARCH},powerpc)
    STATICFLAGS:=-static
    ifndef CROSS_COMPILE
        ERROR_CROSS_COMPILE_NOT_SET:=YES
    endif
    ifndef CROSS_LIB_PATH
        ERROR_CROSS_LIB_PATH_NOT_SET:=YES
    endif
    HOST_COMPILER:=gcc
    COMPILER:=${CROSS_COMPILE}gcc
    EXTRA_CFLAGS += -DARCH_POWERPC
    LIB_PATH:=${CROSS_LIB_PATH}
else ifeq (${ARCH},arm)
    STATICFLAGS:=-static
    ifndef CROSS_COMPILE
        ERROR_CROSS_COMPILE_NOT_SET:=YES
    endif
    ifndef CROSS_LIB_PATH
        ERROR_CROSS_LIB_PATH_NOT_SET:=YES
    endif
    HOST_COMPILER:=gcc
    COMPILER:=${CROSS_COMPILE}gcc
    EXTRA_CFLAGS += -DARCH_ARM
    LIB_PATH:=${CROSS_LIB_PATH}
else ifeq (${ARCH},arm64)
    STATICFLAGS:=-static
    ifndef CROSS_COMPILE
        ERROR_CROSS_COMPILE_NOT_SET:=YES
    endif
    ifndef CROSS_LIB_PATH
        ERROR_CROSS_LIB_PATH_NOT_SET:=YES
    endif
    HOST_COMPILER:=gcc
    COMPILER:=${CROSS_COMPILE}gcc
    EXTRA_CFLAGS += -DARCH_ARM64
    LIB_PATH:=${CROSS_LIB_PATH}
else
    ERROR_ARCH_NOT_SET:=YES
endif

ifeq ($(PATH_DF_LIB),)
    PATH_DF_LIB=../../../Examples/Driver/build
endif

# paths to DDK-internal components
PATH_DRIVER:=$(PATH_DF_LIB)

# relative paths
SRC_TOOL:=../src
SRC_ADAPTER:=../../../Integration/Adapter_GlobalControl/src
SRC_KIT:=../$(PATH_KIT)

INCLUDE_KIT:= \
  -I$(PATH_KIT)/DriverFramework/Basic_Defs_API/incl \
  -I$(PATH_KIT)/DriverFramework/CLib_Abstraction_API/incl \
  -I$(PATH_KIT)/DriverFramework/Device_API/incl \
  -I$(PATH_KIT)/Log/incl \
  -I$(PATH_KIT)/Log/src/printf

INCLUDE_USER:= \
  -I$(PATH_KIT)/Log/src/printf \
  -I$(PATH_UMDEVXS_PROXY)/incl \
  -I$(PATH_UMDEVXS_DRV)/incl \
  -I$(PATH_INT_DSP)/incl \
  -I$(PATH_WRK_TRD)/incl\
  -I$(PATH_DRIVER) \
  -I$(SRC_ADAPTER) \
  -I$(SRC_ADAPTER)/../incl

PATH_MEMXS_BIN := .
PATH_MEMXS_SRC := ../src

OBJS_U = \
	memxs.o\
	adapter_memxs.o

INCLUDE_ALL:= \
  $(SYSTEMTEST_CONFIGURATION) \
  -I$(PATH_MEMXS_SRC) \
  -I$(PATH_MEMXS_BIN) \
  -I$(PATH_CONFIG) \
  -I$(PATH_CONFIG)/$(HOST_HW_PLATFORM) \
  $(INCLUDE_KIT) \
  $(INCLUDE_USER)

MEMXS_CFLAGS +=  -Wall

ifdef USER_CFLAGS
    MEMXS_CFLAGS += ${USER_CFLAGS}
endif

MEMXS_CFLAGS += $(INCLUDE_ALL)

ifeq (${DEBUG_DDK},y)
    MEMXS_CFLAGS += -g
    MEMXS_CFLAGS += -DDEBUG
endif

ifeq (${COVER_DDK},y)
    MEMXS_CFLAGS += -g
    MEMXS_CFLAGS += -ftest-coverage
    MEMXS_CFLAGS += -fprofile-arcs
    USER_LFLAGS += -lgcov
endif

ifeq (${PROFILE_DDK},y)
    MEMXS_CFLAGS += -pg
    USER_LFLAGS += -lgcov
endif

all : memxs

clean:
	-rm -rf *.o  *_u memxs

memxs: $(OBJS_U)
	$(COMPILER) $(MEMXS_CFLAGS) -o memxs $(OBJS_U) \
	-L$(LIB_PATH) -L$(PATH_DF_LIB) -l$(LIB_DF_NAME) -lc $(USER_LFLAGS)

memxs_mod.o: $(PATH_MEMXS_SRC)/memxs_mod.c
	$(COMPILER) $(MEMXS_CFLAGS) -c -o $@ $<

adapter_memxs.o: $(SRC_ADAPTER)/um/adapter_memxs.c
	$(COMPILER) $(MEMXS_CFLAGS) -c -o $@ $<

memxs.o: $(PATH_MEMXS_SRC)/memxs_testapp.c
	$(COMPILER) $(MEMXS_CFLAGS) -c -o $@ $<

