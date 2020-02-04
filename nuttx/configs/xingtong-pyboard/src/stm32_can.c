/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>

#include <nuttx/can/can.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"

#include "stm32.h"
#include "stm32_can.h"
#include "xingtong-pyboard.h"

#ifdef CONFIG_CAN

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Configuration ********************************************************************/

#if defined(CONFIG_STM32_CAN1) && defined(CONFIG_STM32_CAN2)
#  warning "Both CAN1 and CAN2 are enabled.  Assuming only CAN1."
#  undef CONFIG_STM32_CAN2
#endif

#ifdef CONFIG_STM32_CAN1
#  define CAN_PORT 1
#else
#  define CAN_PORT 2
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_can_setup
 *
 * Description:
 *  Initialize CAN and register the CAN device
 *
 ************************************************************************************/

int stm32_can_setup(void)
{
#if defined(CONFIG_STM32_CAN1) || defined(CONFIG_STM32_CAN2)
  struct can_dev_s *can;
  int ret;

  /* Call stm32_caninitialize() to get an instance of the CAN interface */

  can = stm32_caninitialize(CAN_PORT);
  if (can == NULL)
    {
      canerr("ERROR:  Failed to get CAN interface\n");
      return -ENODEV;
    }

  /* Register the CAN driver at "/dev/can0" */

  ret = can_register("/dev/can0", can);
  if (ret < 0)
    {
      canerr("ERROR: can_register failed: %d\n", ret);
      return ret;
    }

  return OK;
#else
  return -ENODEV;
#endif
}

#endif /* CONFIG_CAN */
