############################################################################
# configs/xingtong-pyboard/src/Makefile
############################################################################

-include $(TOPDIR)/Make.defs

ASRCS =
CSRCS = stm32_boot.c stm32_bringup.c

ifeq ($(CONFIG_CAN),y)
CSRCS += stm32_can.c
endif

ifeq ($(CONFIG_STM32_OTGFS),y)
CSRCS += stm32_usb.c
endif

ifeq ($(CONFIG_STM32_SDIO),y)
CSRCS += stm32_sdio.c
endif

ifeq ($(CONFIG_LIB_BOARDCTL),y)
CSRCS += stm32_appinit.c
ifeq ($(CONFIG_BOARDCTL_RESET),y)
CSRCS += stm32_reset.c
endif
endif

ifeq ($(CONFIG_TIMER),y)
CSRCS += stm32_timer.c
endif

ifeq ($(CONFIG_STM32_ROMFS),y)
CSRCS += stm32_romfs_initialize.c
endif

ifeq ($(CONFIG_BOARDCTL_UNIQUEID),y)
CSRCS += stm32_uid.c
endif

ifeq ($(CONFIG_USBMSC),y)
CSRCS += stm32_usbmsc.c
endif

ifeq ($(CONFIG_PWM), y)
CSRCS += stm32_pwm.c
endif


include $(TOPDIR)/configs/Board.mk
