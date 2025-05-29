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
#define BUTTON_K0_Pin GPIO_PIN_4
#define BUTTON_K0_GPIO_Port GPIOE
#define TOUCH_IRQ_Pin GPIO_PIN_0
#define TOUCH_IRQ_GPIO_Port GPIOC
#define TOUCH_IRQ_EXTI_IRQn EXTI0_IRQn
#define SPINDLE_ENC_Pin GPIO_PIN_1
#define SPINDLE_ENC_GPIO_Port GPIOC
#define SPINDLE_ENC_EXTI_IRQn EXTI1_IRQn
#define SPI_TOUCH_MISO_Pin GPIO_PIN_2
#define SPI_TOUCH_MISO_GPIO_Port GPIOC
#define SPI_TOUCH_MOSI_Pin GPIO_PIN_3
#define SPI_TOUCH_MOSI_GPIO_Port GPIOC
#define LONGITUDIAL_FEED_PULSE_Pin GPIO_PIN_0
#define LONGITUDIAL_FEED_PULSE_GPIO_Port GPIOA
#define LONGITUDAL_FEED_EN_Pin GPIO_PIN_1
#define LONGITUDAL_FEED_EN_GPIO_Port GPIOA
#define LONGITUDAL_FEED_DIR_Pin GPIO_PIN_2
#define LONGITUDAL_FEED_DIR_GPIO_Port GPIOA
#define SPI_TFT_CS_Pin GPIO_PIN_4
#define SPI_TFT_CS_GPIO_Port GPIOA
#define LED_D2_Pin GPIO_PIN_6
#define LED_D2_GPIO_Port GPIOA
#define LED_D3_Pin GPIO_PIN_7
#define LED_D3_GPIO_Port GPIOA
#define SPI_TFT_RESET_Pin GPIO_PIN_4
#define SPI_TFT_RESET_GPIO_Port GPIOC
#define SPI_TFT_DC_RS_Pin GPIO_PIN_5
#define SPI_TFT_DC_RS_GPIO_Port GPIOC
#define SPI_TOUCH_SCK_Pin GPIO_PIN_10
#define SPI_TOUCH_SCK_GPIO_Port GPIOB
#define CROSS_FEED_PULSE_Pin GPIO_PIN_6
#define CROSS_FEED_PULSE_GPIO_Port GPIOC
#define CROSS_FEED_EN_Pin GPIO_PIN_7
#define CROSS_FEED_EN_GPIO_Port GPIOC
#define CROSS_FEED_DIR_Pin GPIO_PIN_8
#define CROSS_FEED_DIR_GPIO_Port GPIOC
#define SPI_TOUCH_CS_Pin GPIO_PIN_6
#define SPI_TOUCH_CS_GPIO_Port GPIOD
#define SPI_LCD_MISO_Pin GPIO_PIN_4
#define SPI_LCD_MISO_GPIO_Port GPIOB
#define SPI_LCD_MOSI_Pin GPIO_PIN_5
#define SPI_LCD_MOSI_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
