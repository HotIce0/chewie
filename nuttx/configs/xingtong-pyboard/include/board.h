/************************************************************************************
 * configs/xingtong-pyboard/include/board.h
 *
 *   Copyright (C) 2019 Hotice0 All rights reserved.
 *   Author: Hotice0 <hotice0@qq.com>
 *
 ************************************************************************************/

#ifndef __CONFIG_XINGTONG_PYBOARD_INCLUDE_BOARD_H
#define __CONFIG_XINGTONG_PYBOARD_INCLUDE_BOARD_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#ifndef __ASSEMBLY__
#  include <stdint.h>
#  include <stdbool.h>
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Clocking *************************************************************************/
/* The STM32F405RGT6 XINGTONG_PYBOARD board features a single 12MHz crystal.  Space is provided
 * for a 32kHz RTC backup crystal, but it is not stuffed.
 *
 * This is the canonical configuration:
 *   System Clock source           : PLL (HSE)
 *   SYSCLK(Hz)                    : 168000000    Determined by PLL configuration
 *   HCLK(Hz)                      : 168000000    (STM32_RCC_CFGR_HPRE)
 *   AHB Prescaler                 : 1            (STM32_RCC_CFGR_HPRE)
 *   APB1 Prescaler                : 4            (STM32_RCC_CFGR_PPRE1)
 *   APB2 Prescaler                : 2            (STM32_RCC_CFGR_PPRE2)
 *   HSE Frequency(Hz)             : 12000000      (STM32_BOARD_XTAL)
 *   PLLM                          : 12            (STM32_PLLCFG_PLLM)
 *   PLLN                          : 336          (STM32_PLLCFG_PLLN)
 *   PLLP                          : 2            (STM32_PLLCFG_PLLP)
 *   PLLQ                          : 7            (STM32_PLLCFG_PLLQ)
 *   Main regulator output voltage : Scale1 mode  Needed for high speed SYSCLK
 *   Flash Latency(WS)             : 5
 *   Prefetch Buffer               : OFF
 *   Instruction cache             : ON
 *   Data cache                    : ON
 *   Require 48MHz for USB OTG FS, : Enabled
 *   SDIO and RNG clock
 */

/* HSI - 16 MHz RC factory-trimmed
 * LSI - 32 KHz RC
 * HSE - On-board crystal frequency is 12MHz
 * LSE - 32.768 kHz
 */

#define STM32_BOARD_XTAL        12000000ul

#define STM32_HSI_FREQUENCY     16000000ul
#define STM32_LSI_FREQUENCY     32000
#define STM32_HSE_FREQUENCY     STM32_BOARD_XTAL
#define STM32_LSE_FREQUENCY     32768

/* Main PLL Configuration.
 *
 * PLL source is HSE
 * PLL_VCO = (STM32_HSE_FREQUENCY / PLLM) * PLLN
 *         = (12,000,000 / 12) * 336
 *         = 336,000,000
 * SYSCLK  = PLL_VCO / PLLP
 *         = 336,000,000 / 2 = 168,000,000
 * USB OTG FS, SDIO and RNG Clock
 *         =  PLL_VCO / PLLQ
 *         = 48,000,000
 */

#define STM32_PLLCFG_PLLM       RCC_PLLCFG_PLLM(12)
#define STM32_PLLCFG_PLLN       RCC_PLLCFG_PLLN(336)
#define STM32_PLLCFG_PLLP       RCC_PLLCFG_PLLP(2)
#define STM32_PLLCFG_PLLQ       RCC_PLLCFG_PLLQ(7)

#define STM32_SYSCLK_FREQUENCY  168000000ul

/* AHB clock (HCLK) is SYSCLK (168MHz) */

#define STM32_RCC_CFGR_HPRE     RCC_CFGR_HPRE_SYSCLK  /* HCLK  = SYSCLK / 1 */
#define STM32_HCLK_FREQUENCY    STM32_SYSCLK_FREQUENCY
#define STM32_BOARD_HCLK        STM32_HCLK_FREQUENCY  /* same as above, to satisfy compiler */

/* APB1 clock (PCLK1) is HCLK/4 (42MHz) */

#define STM32_RCC_CFGR_PPRE1    RCC_CFGR_PPRE1_HCLKd4     /* PCLK1 = HCLK / 4 */
#define STM32_PCLK1_FREQUENCY   (STM32_HCLK_FREQUENCY/4)

/* Timers driven from APB1 will be twice PCLK1 */

#define STM32_APB1_TIM2_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM3_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM4_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM5_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM6_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM7_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM12_CLKIN  (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM13_CLKIN  (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM14_CLKIN  (2*STM32_PCLK1_FREQUENCY)

/* APB2 clock (PCLK2) is HCLK/2 (84MHz) */

#define STM32_RCC_CFGR_PPRE2    RCC_CFGR_PPRE2_HCLKd2     /* PCLK2 = HCLK / 2 */
#define STM32_PCLK2_FREQUENCY   (STM32_HCLK_FREQUENCY/2)

/* Timers driven from APB2 will be twice PCLK2 */

#define STM32_APB2_TIM1_CLKIN   (2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM8_CLKIN   (2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM9_CLKIN   (2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM10_CLKIN  (2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM11_CLKIN  (2*STM32_PCLK2_FREQUENCY)

/* Timer Frequencies, if APBx is set to 1, frequency is same to APBx
 * otherwise frequency is 2xAPBx.
 * Note: TIM1,8 are on APB2, others on APB1
 */

#define BOARD_TIM1_FREQUENCY    STM32_APB2_TIM1_CLKIN
#define BOARD_TIM2_FREQUENCY    STM32_APB1_TIM2_CLKIN
#define BOARD_TIM3_FREQUENCY    STM32_APB1_TIM3_CLKIN
#define BOARD_TIM4_FREQUENCY    STM32_APB1_TIM4_CLKIN
#define BOARD_TIM5_FREQUENCY    STM32_APB1_TIM5_CLKIN
#define BOARD_TIM6_FREQUENCY    STM32_APB1_TIM6_CLKIN
#define BOARD_TIM7_FREQUENCY    STM32_APB1_TIM7_CLKIN
#define BOARD_TIM8_FREQUENCY    STM32_APB2_TIM8_CLKIN
#define BOARD_TIM9_FREQUENCY    STM32_APB2_TIM9_CLKIN
#define BOARD_TIM10_FREQUENCY   STM32_APB2_TIM10_CLKIN
#define BOARD_TIM11_FREQUENCY   STM32_APB2_TIM11_CLKIN
#define BOARD_TIM12_FREQUENCY   STM32_APB1_TIM12_CLKIN
#define BOARD_TIM13_FREQUENCY   STM32_APB1_TIM13_CLKIN
#define BOARD_TIM14_FREQUENCY   STM32_APB1_TIM14_CLKIN

/* SDIO dividers.  Note that slower clocking is required when DMA is disabled
 * in order to avoid RX overrun/TX underrun errors due to delayed responses
 * to service FIFOs in interrupt driven mode.  These values have not been
 * tuned!!!
 *
 * SDIOCLK=48MHz, SDIO_CK=SDIOCLK/(118+2)=400 KHz
 */

#define SDIO_INIT_CLKDIV        (118 << SDIO_CLKCR_CLKDIV_SHIFT)

/* DMA ON:  SDIOCLK=48MHz, SDIO_CK=SDIOCLK/(1+2)=16 MHz
 * DMA OFF: SDIOCLK=48MHz, SDIO_CK=SDIOCLK/(2+2)=12 MHz
 */

#ifdef CONFIG_SDIO_DMA
#  define SDIO_MMCXFR_CLKDIV    (1 << SDIO_CLKCR_CLKDIV_SHIFT)
#else
#  define SDIO_MMCXFR_CLKDIV    (2 << SDIO_CLKCR_CLKDIV_SHIFT)
#endif

/* DMA ON:  SDIOCLK=48MHz, SDIO_CK=SDIOCLK/(1+2)=16 MHz
 * DMA OFF: SDIOCLK=48MHz, SDIO_CK=SDIOCLK/(2+2)=12 MHz
 */

#ifdef CONFIG_SDIO_DMA
#  define SDIO_SDXFR_CLKDIV     (1 << SDIO_CLKCR_CLKDIV_SHIFT)
#else
#  define SDIO_SDXFR_CLKDIV     (2 << SDIO_CLKCR_CLKDIV_SHIFT)
#endif

// UART/USART
#define GPIO_USART1_RX  GPIO_USART1_RX_2       /* PB7 */
#define GPIO_USART1_TX  GPIO_USART1_TX_2       /* PB6  */
#define GPIO_USART2_RX  GPIO_USART2_RX_1       /* PA3 */
#define GPIO_USART2_TX  GPIO_USART2_TX_1       /* PA2  */
#define GPIO_USART2_CTS GPIO_USART2_CTS_1      /* PA0 */
#define GPIO_USART2_RTS GPIO_USART2_RTS_1      /* PA1 */
#define GPIO_USART3_RX  GPIO_USART3_RX_1       /* PB11 */
#define GPIO_USART3_TX  GPIO_USART3_TX_1       /* PB10 */
#define GPIO_USART3_CTS GPIO_USART3_CTS_1      /* PB13 */
#define GPIO_USART3_RTS GPIO_USART3_RTS_1      /* PB14 */
#define GPIO_UART4_RX  GPIO_UART4_RX_1         /* PA1 */
#define GPIO_UART4_TX  GPIO_UART4_TX_1         /* PA0 */
#define GPIO_USART6_RX  GPIO_USART6_RX_1       /* PC7 */
#define GPIO_USART6_TX  GPIO_USART6_TX_1       /* PC6 */

/* UART RX DMA configurations */
#define DMAMAP_USART1_RX DMAMAP_USART1_RX_2
#define DMAMAP_USART6_RX DMAMAP_USART6_RX_1

// I2C busses
#define GPIO_I2C1_SCL     GPIO_I2C1_SCL_1      /* PB6 */
#define GPIO_I2C1_SDA     GPIO_I2C1_SDA_1      /* PB7 */
#define GPIO_I2C2_SCL     GPIO_I2C2_SCL_1      /* PB10 */
#define GPIO_I2C2_SDA     GPIO_I2C2_SDA_1      /* PB11 */

// SPI busses 
#define GPIO_SPI1_MISO    GPIO_SPI1_MISO_1  /* PA6 */
#define GPIO_SPI1_MOSI    GPIO_SPI1_MOSI_1  /* PA7 */
#define GPIO_SPI1_NSS     GPIO_SPI1_NSS_2   /* PA4 */
#define GPIO_SPI1_SCK     GPIO_SPI1_SCK_1   /* PA5 */
#define GPIO_SPI2_MISO    GPIO_SPI2_MISO_1  /* PB14 */
#define GPIO_SPI2_MOSI    GPIO_SPI2_MOSI_1  /* PB15 */
#define GPIO_SPI2_NSS     GPIO_SPI2_NSS_1   /* PB12 */
#define GPIO_SPI2_SCK     GPIO_SPI2_SCK_2   /* PB13 */

// CAN busses
#  define GPIO_CAN1_RX GPIO_CAN1_RX_2       /* PB8 */
#  define GPIO_CAN1_TX GPIO_CAN1_TX_2       /* PB9 */
#  define GPIO_CAN2_RX GPIO_CAN2_RX_1       /* PB12 */
#  define GPIO_CAN2_TX GPIO_CAN2_TX_1       /* PB13 */

/* microSD Connector:
 *
 *   ----------------- ----------------- ------------------------
 *   SD/MMC CONNECTOR        BOARD        GPIO CONFIGURATION(s
 *   PIN SIGNAL             SIGNAL          (no remapping)
 *   --- ------------- ----------------- -------------------------
 *   1   DAT2/RES      SD_D2/USART3_TX/  PC10 GPIO_SDIO_D2
 *                     SPI3_SCK
 *   2   CD/DAT3/CS    SD_D3/USART3_RX/  PC11 GPIO_SDIO_D3
 *                     SPI3_MISO
 *   3   CMD/DI        SD_CMD            PD2  GPIO_SDIO_CMD
 *   4   VDD           N/A               N/A
 *   5   CLK/SCLK      SD_CLK/SPI3_MOSI  PC12 GPIO_SDIO_CK
 *   6   VSS           N/A               N/A
 *   7   DAT0/D0       SD_D0/DCMI_D2     PC8  GPIO_SDIO_D0
 *   8   DAT1/RES      SD_D1/DCMI_D3     PC9  GPIO_SDIO_D1
 *   --- ------------- ----------------- -------------------------
 *
 *   NOTES:
 *   1. DAT4, DAT4, DAT6, and DAT7 not connected.
 *   2. There are no alternative pin selections.
 *   3. There is no card detect (CD) GPIO input so we will not
 *      sense if there is a card in the SD slot or not.  This will
 *      make usage very awkward.
 */

/* DMA Channl/Stream Selections *****************************************************/
/* Stream selections are arbitrary for now but might become important in the future
 * if we set aside more DMA channels/streams.
 *
 * SDIO DMA
 *   DMAMAP_SDIO_1 = Channel 4, Stream 3
 *   DMAMAP_SDIO_2 = Channel 4, Stream 6
 */

#define DMAMAP_SDIO DMAMAP_SDIO_1


/* PWM - motor outputs */

#define PWM_TIMER 2

#define GPIO_TIM2_CH1OUT  GPIO_TIM2_CH1OUT_1   /* S1_OUT  PA0 */
#define GPIO_TIM2_CH2OUT  GPIO_TIM2_CH2OUT_1   /* S2_OUT  PA1 */
#define GPIO_TIM2_CH3OUT  GPIO_TIM2_CH3OUT_1   /* S3_OUT  PA2 */
#define GPIO_TIM2_CH4OUT  GPIO_TIM2_CH4OUT_1   /* S4_OUT  PA3 */

#endif  /* __CONFIG_XINGTONG_PYBOARD_INCLUDE_BOARD_H */
