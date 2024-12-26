# jdg_controller.mk
JDG_CONTROLLER_VERSION = 1.0
JDG_CONTROLLER_SITE = /mnt/c/Users/sebas/OneDrive\ -\ Université\ du\ Québec\ à\ Trois-Rivières/Implication/JDG/Machine\ 2025/JDG2025
JDG_CONTROLLER_SITE_METHOD = local
JDG_CONTROLLER_LICENSE = GPL-2.0+
JDG_CONTROLLER_LICENSE_FILES = LICENSE
JDG_CONTROLLER_DEPENDENCIES = BR2_PACKAGE_JDG_WIRINGPI

define JDG_CONTROLLER_BUILD_CMDS
    $(MAKE) -C $(JDG_CONTROLLER_SITE) \
        CC=$(TARGET_CROSS)gcc \
        controller
endef

define JDG_CONTROLLER_INSTALL_TARGET_CMDS
    $(INSTALL) -m 0755 $(JDG_CONTROLLER_SITE)/build/controller $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))
