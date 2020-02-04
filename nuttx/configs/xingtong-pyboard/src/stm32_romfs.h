#ifndef __CONFIGS_XINGTONG_PYBOARD_SRC_STM32_ROMFS_H
#define __CONFIGS_XINGTONG_PYBOARD_SRC_STM32_ROMFS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#ifdef CONFIG_STM32_ROMFS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define ROMFS_SECTOR_SIZE 64

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_romfs_initialize
 *
 * Description:
 *   Registers built-in ROMFS image as block device and mounts it.
 *
 * Returned Value:
 *   Zero (OK) on success, a negated errno value on error.
 *
 * Assumptions/Limitations:
 *   Memory addresses [&romfs_data_begin .. &romfs_data_begin) should contain
 *   ROMFS volume data, as included in the assembly snippet above (l. 84).
 *
 ****************************************************************************/

int stm32_romfs_initialize(void);

#endif /* CONFIG_STM32_ROMFS */

#endif /* __CONFIGS_XINGTONG_PYBOARD_SRC_STM32_ROMFS_H */
