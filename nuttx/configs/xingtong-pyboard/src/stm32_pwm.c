/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>
#include <string.h>
#include <limits.h>

#include <nuttx/drivers/pwm.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "stm32_pwm.h"
#include "xingtong-pyboard.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_pwm_setup
 *
 * Description:
 *
 *   Initialize PWM and register TIM2 PWM devices:
 *
 *    TIM2 CH1 PA0 S1_OUT
 *    TIM2 CH2 PA1 S2_OUT
 *    TIM2 CH3 PA2 S3_OUT
 *    TIM2 CH4 PA3 S4_OUT
 *
 ****************************************************************************/

int stm32_pwm_setup(void) {
	int ret = 0;
	struct pwm_lowerhalf_s *pwm = NULL; /* lower-half driver handle */

	pwm = stm32_pwminitialize(PWM_TIMER);
	if (!pwm){
		aerr("ERROR: Failed to get stm32 PWM lower half\n");
		return ENODEV;
	}

	ret = pwm_register("/dev/pwm_out0", pwm);
	if (ret < 0) {
		aerr("ERROR: PWM register failed : %d\n", ret);
		return ret;
	}
	return 0;
}
