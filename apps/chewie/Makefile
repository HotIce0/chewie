-include $(TOPDIR)/Make.defs

# built-in application info

CONFIG_CHEWIE_PRIORITY ?= SCHED_PRIORITY_DEFAULT
CONFIG_CHEWIE_STACKSIZE ?= 2048

APPNAME = chewie

PRIORITY  = $(CONFIG_CHEWIE_PRIORITY)
STACKSIZE = $(CONFIG_CHEWIE_STACKSIZE)

ASRCS = 
CSRCS = 
MAINSRC = chewie_main.c

# drivers
CSRCS += drivers/motor/drv_motor.c

CSRCS += drivers/pwm_out/drv_pwm_out.c

CONFIG_CHEWIE_PROGNAME ?= chewie$(EXEEXT)
PROGNAME = $(CONFIG_CHEWIE_PROGNAME)

MODULE = CONFIG_CHEWIE

include $(APPDIR)/Application.mk
