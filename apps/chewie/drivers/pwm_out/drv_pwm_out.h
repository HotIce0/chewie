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
 * The path of the pwm out device is named like,
 * CHEWIE_PWM_OUT_DEFAULT_DEVICE_PATH_PREFIX + "0" | + "1" ...
 * /dev/pwm_out0
 * /dev/pwm_out1
 */
#define CHEWIE_PWM_OUT_DEFAULT_DEVICE_PATH_PREFIX "/dev/pwm_out"
/**
 * The value of pwm device frequency(Hz).
 */
#define CHEWIE_PWM_FREQUENCY 400
/**
 * ESC (Electronic Speed Control) relate.
 */
#define CHEWIE_PWM_ESC_ARMED_PULSE_US 900
#define CHEWIE_PWM_ESC_MIN_PULSE_US 1000
#define CHEWIE_PWM_ESC_MAX_PULSE_US 2000

#define CHEWIE_PWM_ARMED_DUTY
#define CHEWIE_NUM_OF_PWM_DEV ((CONFIG_CHEWIE_NUM_OF_CHANNELS % 4) == 0 ? CONFIG_CHEWIE_NUM_OF_CHANNELS / 4 : CONFIG_CHEWIE_NUM_OF_CHANNELS / 4 + 1)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct pwm_state_s
{
  bool initialized;
  struct pwm_info_s pwms_info[CHEWIE_NUM_OF_PWM_DEV];
  int pwm_devs_fd[CHEWIE_NUM_OF_PWM_DEV];
  uint32_t freq;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct pwm_state_s g_pwmstate;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * init pwm device
 */
int init_pwm_device();

/**
 * set current duty of pwm channel
 */
int set_pwm_duty(uint8_t channel, ub16_t duty);


int uninit_pwm_device();