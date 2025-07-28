SDK_INSTALL_DIR = $(abspath ../../..)
include $(SDK_INSTALL_DIR)/imports.mak

# Project name and device family
DEVICE_FAMILY := cc27xx
PROJECT_NAME  := production_full

PROJECT_PATH := $(SDK_INSTALL_DIR)/tfm_s/$(DEVICE_FAMILY)/$(PROJECT_NAME)

# Add extra manifest list files and partition paths
TFM_EXTRA_OPTIONS := \
-DTFM_EXTRA_MANIFEST_LIST_FILES=$(PROJECT_PATH)/tfm_manifest_list.yaml \
-DTFM_EXTRA_PARTITION_PATHS=$(PROJECT_PATH)/

# Override default options with custom options and pass additional options
CONFIGURABLE_OPTIONS := \
-DPROJECT_CONFIG_HEADER_FILE=$(PROJECT_PATH)/config/config_tfm_project.h \
-DTFM_PARTITION_INITIAL_ATTESTATION=True

all: build

# Include the common makefile, which defines the build targets
include $(SDK_INSTALL_DIR)/tfm_s/util/makefile_common.mak
