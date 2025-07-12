/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_POWER_Pin GPIO_PIN_2
#define MOTOR_POWER_GPIO_Port GPIOE
#define TOUCH_IRQ_Pin GPIO_PIN_0
#define TOUCH_IRQ_GPIO_Port GPIOC
#define TOUCH_IRQ_EXTI_IRQn EXTI0_IRQn
#define SPI_TOUCH_MISO_Pin GPIO_PIN_2
#define SPI_TOUCH_MISO_GPIO_Port GPIOC
#define SPI_TOUCH_MOSI_Pin GPIO_PIN_3
#define SPI_TOUCH_MOSI_GPIO_Port GPIOC
#define ENC_A_MEASURE_CROSS_FEED_Pin GPIO_PIN_0
#define ENC_A_MEASURE_CROSS_FEED_GPIO_Port GPIOA
#define ENC_B_MEASURE_CROSS_FEED_Pin GPIO_PIN_1
#define ENC_B_MEASURE_CROSS_FEED_GPIO_Port GPIOA
#define POTENTIOMETER_Pin GPIO_PIN_2
#define POTENTIOMETER_GPIO_Port GPIOA
#define SPI_TFT_CS_Pin GPIO_PIN_4
#define SPI_TFT_CS_GPIO_Port GPIOA
#define SPI_TFT_CLK_Pin GPIO_PIN_5
#define SPI_TFT_CLK_GPIO_Port GPIOA
#define LED_D2_Pin GPIO_PIN_6
#define LED_D2_GPIO_Port GPIOA
#define LED_D3_Pin GPIO_PIN_7
#define LED_D3_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_4
#define TFT_RESET_GPIO_Port GPIOC
#define TFT_DC_RS_Pin GPIO_PIN_5
#define TFT_DC_RS_GPIO_Port GPIOC
#define CROSS_FEED_PULSE_Pin GPIO_PIN_0
#define CROSS_FEED_PULSE_GPIO_Port GPIOB
#define SPI_TFT2_CS_Pin GPIO_PIN_7
#define SPI_TFT2_CS_GPIO_Port GPIOE
#define LONGITUDAL_FEED_PULSE_Pin GPIO_PIN_9
#define LONGITUDAL_FEED_PULSE_GPIO_Port GPIOE
#define SPI_TOUCH_CLK_Pin GPIO_PIN_10
#define SPI_TOUCH_CLK_GPIO_Port GPIOB
#define BUTTON_UP_Pin GPIO_PIN_15
#define BUTTON_UP_GPIO_Port GPIOB
#define BUTTON_DOWN_Pin GPIO_PIN_9
#define BUTTON_DOWN_GPIO_Port GPIOD
#define BUTTON_LEFT_Pin GPIO_PIN_10
#define BUTTON_LEFT_GPIO_Port GPIOD
#define BUTTON_RIGHT_Pin GPIO_PIN_11
#define BUTTON_RIGHT_GPIO_Port GPIOD
#define ENC_B_PANEL_Pin GPIO_PIN_12
#define ENC_B_PANEL_GPIO_Port GPIOD
#define ENC_A_PANEL_Pin GPIO_PIN_13
#define ENC_A_PANEL_GPIO_Port GPIOD
#define PANEL_ENC_BUTTON_Pin GPIO_PIN_14
#define PANEL_ENC_BUTTON_GPIO_Port GPIOD
#define CROSS_FEED_EN_Pin GPIO_PIN_15
#define CROSS_FEED_EN_GPIO_Port GPIOD
#define ENC_A_SPINDLE_Pin GPIO_PIN_6
#define ENC_A_SPINDLE_GPIO_Port GPIOC
#define ENC_B_SPINDLE_Pin GPIO_PIN_7
#define ENC_B_SPINDLE_GPIO_Port GPIOC
#define CROSS_FEED_DIR_Pin GPIO_PIN_8
#define CROSS_FEED_DIR_GPIO_Port GPIOC
#define LONGITUDAL_FEED_DIR_Pin GPIO_PIN_9
#define LONGITUDAL_FEED_DIR_GPIO_Port GPIOC
#define LONGITUDAL_FEED_EN_Pin GPIO_PIN_8
#define LONGITUDAL_FEED_EN_GPIO_Port GPIOA
#define SWITCH_TOUCH_LOCK_Pin GPIO_PIN_11
#define SWITCH_TOUCH_LOCK_GPIO_Port GPIOA
#define ABS_POS_CS_Pin GPIO_PIN_12
#define ABS_POS_CS_GPIO_Port GPIOA
#define ENC_A_MEASURE_LONGITUDAL_FEED_Pin GPIO_PIN_15
#define ENC_A_MEASURE_LONGITUDAL_FEED_GPIO_Port GPIOA
#define BUTTON_OK_Pin GPIO_PIN_11
#define BUTTON_OK_GPIO_Port GPIOC
#define SWITCH_GO_TO_MODE_Pin GPIO_PIN_0
#define SWITCH_GO_TO_MODE_GPIO_Port GPIOD
#define SWITCH_HIGH_SPEED_Pin GPIO_PIN_1
#define SWITCH_HIGH_SPEED_GPIO_Port GPIOD
#define SPI_TOUCH_CS_Pin GPIO_PIN_6
#define SPI_TOUCH_CS_GPIO_Port GPIOD
#define ENC_B_MEASURE_LONGITUDAL_FEED_Pin GPIO_PIN_3
#define ENC_B_MEASURE_LONGITUDAL_FEED_GPIO_Port GPIOB
#define SPI_LCD_MISO_Pin GPIO_PIN_4
#define SPI_LCD_MISO_GPIO_Port GPIOB
#define SPI_LCD_MOSI_Pin GPIO_PIN_5
#define SPI_LCD_MOSI_GPIO_Port GPIOB
#define SPEAKER_Pin GPIO_PIN_0
#define SPEAKER_GPIO_Port GPIOE
#define SWITCH_RUN_Pin GPIO_PIN_1
#define SWITCH_RUN_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
