# jdg_blackbox.mk
JDG_BLACKBOX_VERSION = 1.0
JDG_BLACKBOX_SITE = /mnt/c/Users/sebas/OneDrive\ -\ Université\ du\ Québec\ à\ Trois-Rivières/Implication/JDG/Machine\ 2025/JDG2025
JDG_BLACKBOX_SITE_METHOD = local
JDG_BLACKBOX_LICENSE = GPL-2.0+
JDG_BLACKBOX_LICENSE_FILES = LICENSE
JDG_BLACKBOX_DEPENDENCIES = BR2_PACKAGE_JDG_WIRINGPI

define JDG_BLACKBOX_BUILD_CMDS
    $(MAKE) -C $(JDG_BLACKBOX_SITE) \
        CC=$(TARGET_CROSS)gcc \
        blackbox
endef

define JDG_BLACKBOX_INSTALL_TARGET_CMDS
    $(INSTALL) -m 0755 $(JDG_BLACKBOX_SITE)/build/blackbox $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))