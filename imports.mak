#
# Set location of various cgtools
#
# These variables can be set here or on the command line.  Paths must not
# have spaces.
#
# The various *_ARMCOMPILER variables, in addition to pointing to
# their respective locations, also serve as "switches" for disabling a build
# using those cgtools. To disable a build using a specific cgtool, either set
# the cgtool's variable to empty or delete/comment-out its definition:
#     TICLANG_ARMCOMPILER ?=
# or
#     #TICLANG_ARMCOMPILER ?= ...
#
# If a cgtool's *_ARMCOMPILER variable is set (non-empty), various sub-makes
# in the installation will attempt to build with that cgtool.  This means
# that if multiple *_ARMCOMPILER cgtool variables are set, the sub-makes
# will build using each non-empty *_ARMCOMPILER cgtool.
#

SYSCONFIG_TOOL         ?= /home/username/ti/ccs2010/ccs/utils/sysconfig_1.23.1/sysconfig_cli.sh

CMAKE                  ?= /home/username/cmake-3.21.3/bin/cmake
PYTHON                 ?= python3

TICLANG_ARMCOMPILER    ?= /home/username/ti/ccs2010/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS
GCC_ARMCOMPILER        ?= /home/username/arm-none-eabi-gcc/12.3.Rel1-0
IAR_ARMCOMPILER        ?= /home/username/iar9.60.3

# Uncomment this to enable the TFM build
# ENABLE_TFM_BUILD=1

ifeq ("$(SHELL)","sh.exe")
    # for Windows/DOS shell

    # Note that sadly Windows' del command can't handle forward slashes so the
    # SLASH_FIXUP function is used to insulate portable makefiles from that.
    # Typically this is only used for clean goals (where 'del' is used), and
    # only when removing files specified with full, forward-slash-containing
    # paths.
    SLASH_FIXUP = $(subst /,\,$1)

    RM      = del
    RMDIR   = -rmdir /S /Q
    DEVNULL = NUL
    ECHOBLANKLINE = @cmd /c echo.
else
    # for Linux-like shells
    SLASH_FIXUP = $1

    RM      = rm -f
    RMDIR   = rm -rf
    DEVNULL = /dev/null
    ECHOBLANKLINE = echo
endif
