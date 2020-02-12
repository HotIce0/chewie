#include <nuttx/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>
#include <string.h>

#include <nuttx/drivers/pwm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/**
 * ESC (Electronic Speed Control) relate.
 */
//#define CHEWIE_PWM_ESC_ARMED_PULSE_US 900
//#define CHEWIE_PWM_ESC_MIN_PULSE_US 1000
//#define CHEWIE_PWM_ESC_MAX_PULSE_US 2000

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 * init pwm device
 */
int drv_pwm_init(const char *devpath);

int drv_pwm_start(int fd);

int drv_pwm_stop(int fd);

int drv_pwm_update_info(int fd, const struct pwm_info_s *p_init_info_s);

void drv_pwm_uninit(int fd);