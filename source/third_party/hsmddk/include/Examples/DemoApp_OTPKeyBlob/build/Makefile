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
##  Makefile for Demo Application for VAL API and OTP Key Blob generation
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

	# Set PATH_DRIVER_VIP/VAL_SYMVERS to the Driver VIP/VAL build directory where
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
    OBJCOPY = ${CROSS_COMPILE}objcopy
    ifneq (,$(findstring little-endian,${USER_CFLAGS}))
        OBJ_FORMAT = elf32-microblazeel
    else
        OBJ_FORMAT = elf32-microblaze
    endif
    OBJ_ARCH = MicroBlaze
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

DA_OTPKEYBLOB_NAME_U:=da_otpkeyblob_u
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

# Include Paths
INCLUDE_KIT:= \
  -I$(PATH_BASE)/$(PATH_KIT)/DriverFramework/Basic_Defs_API/incl \
  -I$(PATH_BASE)/$(PATH_KIT)/DriverFramework/CLib_Abstraction_API/incl \
  -I$(PATH_BASE)/$(PATH_KIT)/Log/incl \
  -I$(PATH_BASE)/$(PATH_KIT)/EIP130/TokenHelper/incl

INCLUDE_DRIVER_API:= \
  -I$(PATH_BASE)/$(PATH_DRIVER_API) \
  -I$(PATH_BASE)/$(PATH_DRIVER_INIT_API)

INCLUDE_DEMOAPP:= \
  $(SYSTEMTEST_CONFIGURATION) \
  -I$(PWD) \
  -I$(PATH_BASE)/$(PATH_CONFIG) \
  -I$(PATH_BASE)/$(PATH_CONFIG)/$(HOST_HW_PLATFORM)

ifeq (${ARCH},mb)
    INCLUDE_USER:=-I$(PATH_BASE)/$(PATH_KIT)/Log/src/xilprintf
else
    INCLUDE_USER:=-I$(PATH_BASE)/$(PATH_KIT)/Log/src/printf
endif

INCLUDE_ALL:= \
  $(INCLUDE_KIT) \
  $(INCLUDE_DRIVER_API) \
  $(INCLUDE_DEMOAPP)

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

USER_CFLAGS += \
  $(USER_FLAGS_WARN) \
  $(INCLUDE_USER)

.PHONY = clean clean_module combined

ifeq (${ARCH},mb)
    USER_CFLAGS += -DDA_OTPKEYBLOB_MAIN_REMOVE
endif

ifeq (${ARCH},mb)
    DA_OTPKEYBLOB_BUILD_RULES = asset keyblob $(DA_OTPKEYBLOB_NAME_U)
else
    DA_OTPKEYBLOB_BUILD_RULES = $(DA_OTPKEYBLOB_NAME_U)
endif

combined: default

default: $(DA_OTPKEYBLOB_BUILD_RULES)

asset:
	dd if=/dev/urandom of=da_otpkeyblob_asset.bin bs=1K count=10
	$(OBJCOPY) -I binary -O $(OBJ_FORMAT) -B $(OBJ_ARCH) da_otpkeyblob_asset.bin da_otpkeyblob_asset.o

keyblob:
	dd if=/dev/urandom of=da_otpkeyblob_keyblob.bin bs=1K count=1
	$(OBJCOPY) -I binary -O $(OBJ_FORMAT) -B $(OBJ_ARCH) da_otpkeyblob_keyblob.bin da_otpkeyblob_keyblob.o

# DemoApp for user-space
$(DA_OTPKEYBLOB_NAME_U):
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
	$(COMPILER) -c ../src/*.c $(USER_CFLAGS) $(EXTRA_CFLAGS)
	$(COMPILER) -c $(PATH_BASE)/$(PATH_KIT)/Log/src/log.c $(USER_CFLAGS) $(EXTRA_CFLAGS)
	@if [ "${ARCH}" = "mb" ] ; then \
	    OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	    $(AR) -cq$(AR_VERBOSE) libda_otpkeyblob_combined_u.a $$OBJLIST ; \
	fi
	@if [ "${ARCH}" != "mb" -a -f $(PATH_DRIVER_VIP_SYMVERS)/lib$(DRIVER_VIP_NAME_U).a ]; then \
	    OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	    $(COMPILER) -o da_otpkeyblob_combined_u $$OBJLIST \
	        -L$(LIB_PATH) -L$(PATH_DRIVER_VIP_SYMVERS) $(DRIVERLIB_VIP) \
	        $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	fi
	@if [ "${ARCH}" != "mb" -a -f $(PATH_DRIVER_VAL_SYMVERS)/lib$(DRIVER_VAL_NAME_U).a ]; then \
	    OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	    $(COMPILER) -o da_otpkeyblob_u $$OBJLIST \
	        -L$(LIB_PATH) -L$(PATH_DRIVER_VAL_SYMVERS) $(DRIVERLIB_VAL) \
	        $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	fi
	@if [ "${ARCH}" != "mb" -a -f $(PATH_DRIVER_VIP_SYMVERS)/lib$(DRIVER_VIP_NAME_U)p.a ]; then \
	    OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	    $(COMPILER) -o da_otpkeyblob_combined_up $$OBJLIST \
	        -L$(LIB_PATH) -L$(PATH_DRIVER_VIP_SYMVERS) $(DRIVERLIB_VIP)p \
	        $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	fi
	@if [ "${ARCH}" != "mb" -a -f $(PATH_DRIVER_VAL_SYMVERS)/lib$(DRIVER_VAL_NAME_U)p.a ]; then \
	    OBJLIST=`ls *.o |grep -v "_k\.o"|grep -v "\.mod\.o"` ; \
	    $(COMPILER) -o da_otpkeyblob_up $$OBJLIST \
	        -L$(LIB_PATH) -L$(PATH_DRIVER_VAL_SYMVERS) $(DRIVERLIB_VAL)p \
	        $(EARLY_USER_LFLAGS) -lpthread -lc $(USER_LFLAGS) ; \
	fi

# Cleaning
clean:	clean_module

clean_module:
	rm -rf *.o .*.cmd
	rm -f ../src/*.o ../src/.*.cmd
	rm -f da_otpkeyblob_* *.a *.bin
	rm -f *.gcno *.gcda *.out

# end of file
