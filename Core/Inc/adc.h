/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define ADC_CHANNELS    6
#define ADC_BIN_SHIFT   5
#define ADC_SAMPLES     (1<<ADC_BIN_SHIFT)
#define ADC_DATA_SIZE   (ADC_CHANNELS * ADC_SAMPLES)
#define ADC_BUFFER_SIZE (ADC_DATA_SIZE * 2)

// Константы из даташита STM32F103
#define VREFINT_TYP          1.20f     // Типовое внутреннее опорное напряжение (В)
#define V25                  1.43f     // Напряжение датчика при 25°C (В)
#define AVG_SLOPE            0.0043f  // Температурный коэффициент (В/°C) - 4.3 мВ/°C

//Каналы АЦП
#define ADC_CH_0    0
#define ADC_CH_1    1
#define ADC_CH_2    2
#define ADC_CH_3    3
#define ADC_TEMP    4
#define ADC_VCC     5
/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void ADC_Start(void);
float Calculate_VDD(uint16_t vrefint_adc);
float Calculate_Temperature(uint16_t temp_adc, float current_vdd);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

