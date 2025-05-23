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
##  Makefile for Test Tool for VAL API
##  How to use :
##  i)    $ make
##  ii)   For cleaning : $ make clean
##############################################################################

#### User Edit Section : Variables to be set by the User
##   User may not need to modify anything other than this section

  ##-->: { These variables in this section are set by the user.

	# Set KERNEL_DIR to the path to the Kernel Source directory for which you
	# wish to compile.
	#KERNEL_DIR:=

	# Set ARCH according to the target architecture
	# supported values: x86, arm, arm64 and powerpc
	#ARCH:=x86
	#ARCH:=arm
	#ARCH:=arm64
	#ARCH:=powerpc

	# Set PATH_DRIVER_VIP_SYMVERS to the Driver VIP build directory where
	# the Module.symvers file is located. If not set then a default build
	# location will be used.

	# Set CROSS_COMPILE to the path to the cross compiler binutils
	# concatenated with the prefix they have before normal binutils name
	# For example :
	# If we have cross compiler for Power-PC in /opt/eldk/target/usr/bin/
	# by the file name ppc_4xxFP-gcc, we set it as below :
	#CROSS_COMPILE:=/opt/eldk/target/usr/bin/

	# Set CROSS_LIB_PATH to the path to the cross compiler libraries
	#CROSS_LIB_PATH:=/opt/eldk/ppc_4xxFP/lib

	# paths to DDK-internal components
	PATH_DRIVER_API:=../../Integration/Adapter_VAL/incl
	PATH_DRIVER_INIT_API:=../../Integration/Adapter_DriverInit/incl
	PATH_ADAPTER_GENERIC_API:=../../Integration/Adapter_Generic/src
	PATH_KIT:=../../Kit
	PATH_CONFIG:=../../Config

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

# cs_*.h files respond to ARCH_X86
ifeq (${ARCH},x86)
    HOST_COMPILER:=gcc
    COMPILER:=gcc
    EXTRA_CFLAGS += -DARCH_X86
    LIB_PATH=/usr/lib
else ifeq (${ARCH},powerpc)
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
else ifeq (${ARCH},mb)
    ifndef CROSS_COMPILE
        ERROR_CROSS_COMPILE_NOT_SET:=YES
    endif
    HOST_COMPILER:=gcc
    COMPILER:=${CROSS_COMPILE}gcc
    AR:=${CROSS_COMPILE}ar
    EXTRA_CFLAGS += -DARCH_MB
    LIB_PATH:=${CROSS_LIB_PATH}
    DA_LIB_PATH:=../../../Examples/DemoApp_VAL/build
    DA_LIB:=-lda_val_combined_u
else
    ERROR_ARCH_NOT_SET:=YES
endif

ifneq (,$(findstring s,${MAKEFLAGS}))
    AR_VERBOSE:=
else
    AR_VERBOSE:=v
endif

ifeq ($(KERNELRELEASE),)
    PWD:= $(shell pwd)
endif

ifeq ($(DDK_PWD_OVERRIDE),)
    DDK_PWD_OVERRIDE := `pwd`
else
    override PWD := $(DDK_PWD_OVERRIDE)
endif

TEST_VAL_NAME_U:=test_val_u
TEST_VAL_NAME_KO:=test_val_k
DRIVER_VIP_NAME_U:=driver_vip_u
DRIVER_VAL_NAME_U:=driver_val_u

DRIVERLIB_VIP=-l$(DRIVER_VIP_NAME_U)
DRIVERLIB_VAL=-l$(DRIVER_VAL_NAME_U)

ifeq ($(PATH_DRIVER_VIP_SYMVERS),)
    PATH_DRIVER_VIP_SYMVERS=$(PWD)/../../../Examples/Driver/build
endif

ifeq ($(PATH_DRIVER_VAL_SYMVERS),)
    PATH_DRIVER_VAL_SYMVERS=$(PWD)/../../../Examples/Driver_VAL/build
endif

PATH_BASE=$(PWD)/..
PATH_TESTTOOLS=$(PWD)/../..

# Include Paths
INCLUDE_KIT:= \
  -I$(PATH_BASE)/$(PATH_KIT)/DriverFramework/Basic_Defs_API/incl \
  -I$(PATH_BASE)/$(PATH_KIT)/DriverFramework/CLib_Abstraction_API/incl \
  -I$(PATH_BASE)/$(PATH_KIT)/Log/incl \
  -I$(PATH_BASE)/$(PATH_KIT)/EIP130/TokenHelper/incl

INCLUDE_DRIVER_API:= \
  -I$(PATH_BASE)/$(PATH_DRIVER_API) \
  -I$(PATH_BASE)/$(PATH_DRIVER_INIT_API)

INCLUDE_TESTAPP:= \
  $(SYSTEMTEST_CONFIGURATION) \
  -I$(PWD) \
  -I$(PATH_BASE)/$(PATH_CONFIG) \
  -I$(PATH_BASE)/$(PATH_CONFIG)/$(HOST_HW_PLATFORM) \
  -I$(PATH_BASE)/ValTests \
  -I$(PATH_TESTTOOLS)/TestVectors/incl \
  -I$(PATH_TESTTOOLS)/TestVectors/src \
  -I$(PATH_BASE)/incl \
  -I$(PATH_BASE)/src

ifeq (${ARCH},mb)
    INCLUDE_USER:=-I$(PATH_BASE)/$(PATH_KIT)/Log/src/xilprintf
else
    INCLUDE_USER:=-I$(PATH_BASE)/$(PATH_KIT)/Log/src/printf
    INCLUDE_KERNEL:= -I$(PATH_BASE)/$(PATH_KIT)/Log/src/printk
endif

INCLUDE_KERNEL += -I$(PATH_BASE)/$(PATH_ADAPTER_GENERIC_API)/lkm
INCLUDE_USER += -I$(PATH_BASE)/$(PATH_ADAPTER_GENERIC_API)/um

INCLUDE_ALL:= \
  $(INCLUDE_KIT) \
  $(INCLUDE_DRIVER_API) \
  $(INCLUDE_TESTAPP)

ifeq (${ARCH},powerpc)
    INCLUDE_ALL += -I$(strip $(KERNEL_DIR))/arch/powerpc
    INCLUDE_ALL += -I$(strip $(KERNEL_DIR))/arch/powerpc/include
    INCLUDE_ALL += -I$(strip $(KERNEL_DIR))/include
endif

ifneq (${ARCH},mb)
    USER_FLAGS_WARN = -W -Wall -Wextra -Wvla -Wlogical-op \
                      -Wstrict-prototypes -Wmissing-prototypes \
                      -Wshadow -Wnested-externs -Wundef -Wmissing-declarations \
                      -Wold-style-definition -Wpointer-arith -Wdouble-promotion \
                      -Wcast-align -Wcast-qual -Wconversion -Wsign-conversion \
                      -Wjump-misses-init -Wwrite-strings -Wformat=2

    USER_FLAGS_WARN += ${WARNING_FLAGS}
# -Wswitch-enum
endif

# Compiler Flags
#DEBUG_FLAGS=
#CODE_COVERAGE_FLAGS=
#PROFILING_FLAGS=
#BACKWARD_COMPATIBILITY_FLAGS=
EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)
UNDER:=_
CONFIG_FLAGS=-D$(word 1, $(subst $(UNDER),$(SPACE),$(HOST_HW_PLATFORM)))

ifeq (${DEBUG_DDK},y)
    DEBUG_FLAGS += -g
    DEBUG_FLAGS += -DDEBUG
endif

ifeq (${COVER_DDK},y)
    DEBUG_FLAGS += -g
    USER_CFLAGS += -ftest-coverage
    USER_CFLAGS += -fprofile-arcs
    USER_LFLAGS += -lgcov
endif

ifeq (${PROFILE_DDK},y)
    USER_CFLAGS += -pg
    USER_LFLAGS += -pg
endif

EXTRA_CFLAGS += \
  $(BACKWARD_COMPATIBILITY_FLAGS) \
  $(INCLUDE_ALL) \
  $(INCLUDE_KERNEL) \
  $(DEBUG_FLAGS) \
  $(CODE_COVERAGE_FLAGS) \
  $(PROFILING_FLAGS) \
  $(CONFIG_FLAGS)
# above INCLUDE_KERNEL will also be present for user-mode builds
# we conquer that by ensuring the user-mode include is first in the path

USER_CFLAGS += \
  $(USER_FLAGS_WARN) \
  $(INCLUDE_USER) \
  -DTEST_VAL_USERMODE

.PHONY = clean clean_module combined

ifeq (${ARCH},mb)
    USER_CFLAGS += -DTEST_VAL_MAIN_REMOVE
    TEST_VAL_BUILD_RULES = $(TEST_VAL_NAME_U)
else
    TEST_VAL_BUILD_RULES = $(TEST_VAL_NAME_KO) $(TEST_VAL_NAME_U)
endif

combined: default

default: $(TEST_VAL_BUILD_RULES)

obj-m:=$(TEST_VAL_NAME_KO).o

valtests-objs:=$(patsubst %c,%o,$(subst $(PWD)/,,$(wildcard $(PATH_BASE)/ValTests/*.c)))
testvectors-objs:=$(patsubst %.c,%.o,$(subst $(PWD)/,,$(wildcard $(PATH_TESTTOOLS)/TestVectors/src/*.c)))
app-objs:=$(patsubst %.c,%.o,$(subst $(PWD)/,,$(wildcard $(PATH_BASE)/src/*.c)))

$(TEST_VAL_NAME_KO)-objs:= \
  ../$(PATH_KIT)/Log/src/log.o \
  $(valtests-objs) \
  $(testvectors-objs) \
  $(app-objs)

# TestTool for kernel-space
KBUILD_EXTRA_SYMBOLS:=$(PATH_DRIVER_VIP_SYMVERS)/Module.symvers
$(TEST_VAL_NAME_KO):
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
	@if [ ! -f $(PATH_DRIVER_VIP_SYMVERS)/Module.symvers ]; then \
	    echo "Failed to locate $(PATH_DRIVER_VIP_SYMVERS)/Module.symvers"; \
	    echo "Please build the driver first"; \
	    exit 1; \
	fi
#	echo "EXTRA_CFLAGS=$(EXTRA_CFLAGS)"
	$(MAKE) DDK_PWD_OVERRIDE=$(DDK_PWD_OVERRIDE) $(EXTRA_KBUILD_FLAGS) ARCH=$(ARCH) CROSS_COMPILE=$(strip $(CROSS_COMPILE)) -C $(KERNEL_DIR) SUBDIRS=$(PWD) M=$(PWD) modules

# TestTool for user-space
$(TEST_VAL_NAME_U):
	@if [ "$(ERROR_ARCH_NOT_SET)" = "YES" ]; then \
	    echo "ARCH not set or unsupported"; \
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
	$(COMPILER) -c ../ValTests/*.c $(USER_CFLAGS) $(EXTRA_CFLAGS)
	$(COMPILER) -c ../../TestVectors/src/*.c $(USER_CFLAGS) $(EXTRA_CFLAGS)
	$(COMPILER) -c ../src/*.c $(USER_CFLAGS) $(EXTRA_CFLAGS)
	$(COMPILER) -c $(PATH_BASE)/$(PATH_KIT)/Log/src/log.c $(USER_CFLAGS) $(EXTRA_CFLAGS)
	@if [ "${ARCH}" = "mb" ] ; then \
	    OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	    $(AR) -cq$(AR_VERBOSE) libtest_val_combined_u.a $$OBJLIST ; \
	else \
	   if [ -f $(PATH_DRIVER_VIP_SYMVERS)/lib$(DRIVER_VIP_NAME_U).a ]; then \
	       OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	       $(COMPILER) -o test_val_combined_u $$OBJLIST \
	           -L$(LIB_PATH) -L$(PATH_DRIVER_VIP_SYMVERS) $(DRIVERLIB_VIP) \
	           $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	   fi ; \
	   if [ -f $(PATH_DRIVER_VAL_SYMVERS)/lib$(DRIVER_VAL_NAME_U).a ]; then \
	       OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	       $(COMPILER) -o $(TEST_VAL_NAME_U) $$OBJLIST \
	           -L$(LIB_PATH) -L$(PATH_DRIVER_VAL_SYMVERS) $(DRIVERLIB_VAL) \
	           $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	   fi ; \
	   if [ -f $(PATH_DRIVER_VIP_SYMVERS)/lib$(DRIVER_VIP_NAME_U)p.a ]; then \
	       OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	       $(COMPILER) -o test_val_combined_up $$OBJLIST \
	           -L$(LIB_PATH) -L$(PATH_DRIVER_VIP_SYMVERS) $(DRIVERLIB_VIP)p \
	           $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	   fi ; \
	   if [ -f $(PATH_DRIVER_VAL_SYMVERS)/lib$(DRIVER_VAL_NAME_U)p.a ]; then \
	       OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	       $(COMPILER) -o $(TEST_VAL_NAME_U)p $$OBJLIST \
	           -L$(LIB_PATH) -L$(PATH_DRIVER_VAL_SYMVERS) $(DRIVERLIB_VAL)p \
	           $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	   fi ; \
	fi

# Cleaning
clean:	clean_module

clean_module:
	rm -rf .tmp_versions *.o .*.cmd .*.flags *.mod.c *.ko *.symvers
	rm -f *.order *.markers test_val_* *.a
	rm -f *.gcno *.gcda *.out
	(cd $(PATH_BASE)/ValTests ; rm -rf *.o .*.o.cmd)
	(cd $(PATH_BASE)/../TestVectors/src ; rm -rf *.o .*.o.cmd)
	(cd $(PATH_BASE)/src ; rm -rf *.o .*.o.cmd)


# end of file
