/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include "stm32_uid.h"

#include <nuttx/board.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef OK
#  define OK 0
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#if defined(CONFIG_BOARDCTL_UNIQUEID)
int board_uniqueid(uint8_t *uniqueid)
{
  if (uniqueid == 0)
    {
      return -EINVAL;
    }

  stm32_get_uniqueid(uniqueid);
  return OK;
}
#endif
