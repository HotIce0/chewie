/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/timers/timer.h>

#include <debug.h>

#include "stm32_tim.h"
#include "xingtong-pyboard.h"

#ifdef CONFIG_TIMER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_timer_driver_setup
 *
 * Description:
 *   Configure the timer driver.
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.  This should be of the form /dev/timer0
 *   timer   - The timer's number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

int stm32_timer_driver_setup(FAR const char *devpath, int timer)
{
  return stm32_timer_initialize(devpath, timer);
}

#endif
