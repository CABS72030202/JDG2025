# external.mk
BR2_EXTERNAL_JDG2025_PATH := JDG2025/config/buildroot
BR2_EXTERNAL += $(BR2_EXTERNAL_JDG2025_PATH)
BR2_GLOBAL_PATCH_DIR += $(BR2_EXTERNAL_JDG2025_PATH)/patches

# Include package recipes
include $(sort $(wildcard $(BR2_EXTERNAL_JDG2025_PATH)/package/*/*.mk))