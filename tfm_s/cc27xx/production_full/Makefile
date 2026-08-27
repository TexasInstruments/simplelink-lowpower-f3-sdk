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
-DTFM_PARTITION_INITIAL_ATTESTATION=True \
-DMBEDTLS_CONFIG_PATH=third_party/mbedtls/ti/configs/config-hsm.h

DEVICE_VARIANTS := cc27xxx10 cc27xxx20

all: $(addprefix build-,$(DEVICE_VARIANTS))

build-%:
	$(MAKE) build TI_DEVICE_VARIANT=$*

clean: $(addprefix clean-,$(DEVICE_VARIANTS))

clean-%:
	@ echo "$(COLOR_BLUE)Cleaning $(DEVICE_FAMILY) $(PROJECT_NAME) $* project...$(COLOR_RESET)"
	@ $(RMDIR) $(call SLASH_FIXUP,$(SDK_INSTALL_DIR)/tfm_s/build/$*/$(PROJECT_NAME))

# Include the common makefile, which defines the build targets
include $(SDK_INSTALL_DIR)/tfm_s/util/makefile_common.mak

# Dispatch trim across all device variants. Defined after include to override
# makefile_common.mak's trim recipe, which would otherwise recurse infinitely
# when trim-% re-invokes $(MAKE) trim with TI_DEVICE_VARIANT set.
ifndef TI_DEVICE_VARIANT
trim: $(addprefix trim-,$(DEVICE_VARIANTS))
	@ :

trim-%:
	$(MAKE) trim TI_DEVICE_VARIANT=$*
endif
