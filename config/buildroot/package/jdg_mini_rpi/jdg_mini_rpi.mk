# jdg_mini_rpi.mk
JDG_MINI_RPI_VERSION = 1.0
JDG_MINI_RPI_SITE = /mnt/c/Users/sebas/OneDrive\ -\ Université\ du\ Québec\ à\ Trois-Rivières/Implication/JDG/Machine\ 2025/JDG2025
JDG_MINI_RPI_SITE_METHOD = local
JDG_MINI_RPI_LICENSE = GPL-2.0+
JDG_MINI_RPI_LICENSE_FILES = LICENSE
JDG_MINI_RPI_DEPENDENCIES = BR2_PACKAGE_JDG_WIRINGPI

define JDG_MINI_RPI_BUILD_CMDS
    $(MAKE) -C $(JDG_MINI_RPI_SITE) \
        CC=$(TARGET_CROSS)gcc \
        mini_rpi
endef

define JDG_MINI_RPI_INSTALL_TARGET_CMDS
    $(INSTALL) -m 0755 $(JDG_MINI_RPI_SITE)/build/mini_rpi $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))
