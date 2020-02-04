/****************************************************************************
 * configs/xingtong-pyboard/src/xingtong-pyboard.h
 ****************************************************************************/

#ifndef __CONFIGS_XINGTONG_PYBOARD_SRC_XINGTONG_PYBOARD_H
#define __CONFIGS_XINGTONG_PYBOARD_SRC_XINGTONG_PYBOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>
#include <stdint.h>
#include <arch/stm32/chip.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef OK
#  define OK 0
#endif

/* How many SPI modules does this chip support? */

#if STM32_NSPI < 1
#  undef CONFIG_STM32_SPI1
#  undef CONFIG_STM32_SPI2
#  undef CONFIG_STM32_SPI3
#elif STM32_NSPI < 2
#  undef CONFIG_STM32_SPI2
#  undef CONFIG_STM32_SPI3
#elif STM32_NSPI < 3
#  undef CONFIG_STM32_SPI3
#endif

#define HAVE_SDIO       1
#define HAVE_RTC_DRIVER 1

/* Can't support MMC/SD features if mountpoints are disabled or if SDIO support
 * is not enabled.
 */

#if defined(CONFIG_DISABLE_MOUNTPOINT) || !defined(CONFIG_STM32_SDIO)
#  undef HAVE_SDIO
#endif

#undef  SDIO_MINOR     /* Any minor number, default 0 */
#define SDIO_SLOTNO 0  /* Only one slot */

#ifdef HAVE_SDIO
#  if !defined(CONFIG_NSH_MMCSDSLOTNO)
#    define CONFIG_NSH_MMCSDSLOTNO SDIO_SLOTNO
#  elif CONFIG_NSH_MMCSDSLOTNO != 0
#    warning "Only one MMC/SD slot, slot 0"
#    undef CONFIG_NSH_MMCSDSLOTNO
#    define CONFIG_NSH_MMCSDSLOTNO SDIO_SLOTNO
#  endif

#  if defined(CONFIG_NSH_MMCSDMINOR)
#    define SDIO_MINOR CONFIG_NSH_MMCSDMINOR
#  else
#    define SDIO_MINOR 0
#  endif
#endif

/* Check if we can support the RTC driver */

#if !defined(CONFIG_RTC) || !defined(CONFIG_RTC_DRIVER)
#  undef HAVE_RTC_DRIVER
#endif

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define STM32_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define STM32_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/* USB OTG FS
 *
 * PC5  OTG_FS_VBUS VBUS sensing
 */

#define GPIO_OTGFS_VBUS   (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|\
                           GPIO_OPENDRAIN|GPIO_PORTA|GPIO_PIN9)

/* MicroSD
 *
 * ---------- ------------- ------------------------------
 * PIO        SIGNAL        Comments
 * ---------- ------------- ------------------------------
 * PA15       NCD           Pulled up externally
 * PC9        DAT1          Configured by driver
 * PC8        DAT0          "        " "" "    "
 * PC12       CLK           "        " "" "    "
 * PD2        CMD           "        " "" "    "
 * PC11       CD/DAT3       "        " "" "    "
 * PC10       DAT2          "        " "" "    "
 * ---------- ------------- ------------------------------
 */

#if defined(CONFIG_STM32_SDIO)
#  define GPIO_SDIO_NCD   (GPIO_INPUT | GPIO_PULLUP | GPIO_EXTI | GPIO_PORTA | GPIO_PIN15)
#endif

/* Configuration ************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public data
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_INITIALIZE=y :
 *     Called from board_initialize().
 *
 *   CONFIG_BOARD_INITIALIZE=y && CONFIG_LIB_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_usbinitialize very early in initialization to setup
 *   USB-related GPIO pins for the STM32F4Discovery board.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_OTGFS
void weak_function stm32_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host
 *   functionality. This function will start a thread that will monitor for
 *   device connection/disconnection events.
 *
 ****************************************************************************/

#if defined(CONFIG_STM32_OTGFS) && defined(CONFIG_USBHOST)
int stm32_usbhost_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_can_setup
 *
 * Description:
 *  Initialize CAN and register the CAN device
 *
 ****************************************************************************/

#ifdef CONFIG_CAN
int stm32_can_setup(void);
#endif

/****************************************************************************
 * Name: stm32_sdio_initialize
 *
 * Description:
 *   Initialize SDIO-based MMC/SD card support
 *
 ****************************************************************************/

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_STM32_SDIO)
int stm32_sdio_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_timer_driver_setup
 *
 * Description:
 *   Configure the timer driver.
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.  This should be of the
 *             form /dev/timer0
 *   timer   - The timer's number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negative errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

#ifdef CONFIG_TIMER
int stm32_timer_driver_setup(FAR const char *devpath, int timer);
#endif

/****************************************************************************
 * Name: stm32_pwm_setup
 * Description:
 * 	Configure the PWM driver.
 *
 * Return Value:
 * 	Zero (OK) is returned on success; A negative errno value is returned
 * 	to indicate the nature of any failure.
 ****************************************************************************/

#ifdef CONFIG_PWM
int stm32_pwm_setup(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __CONFIGS_XINGTONG_PYBOARD_SRC_XINGTONG_PYBOARD_H */
