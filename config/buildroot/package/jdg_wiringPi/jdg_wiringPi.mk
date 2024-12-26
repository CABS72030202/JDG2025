JDG_WIRINGPI_VERSION = 20210904
JDG_WIRINGPI_SOURCE = 7f8fe26e4f775abfced43c07657a353f03ddb2d0.zip
JDG_WIRINGPI_SITE = https://github.com/WiringPi/WiringPi/archive

define JDG_WIRINGPI_BUILD_CMDS
    sh build
endef

$(eval $(generic-package))