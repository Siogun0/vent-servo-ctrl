/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "meta.h"
#include "can_platform.h"
#include "xcp_client_can.h"
#include "xcp_platform.h"
#include "common_param.h"

#include "can_node_valves_bus0.h"
#include "ina219.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define SERVOS_NUMBER   8

typedef struct
{
    uint32_t update_boot;               //0x20003C00
    uint32_t test1;                     //0x20003C04
    uint32_t test2;                     //0x20003C08
    uint32_t test3;                     //0x20003C0C
    uint16_t ADC[8];                    //0x20003C10, 0x20003C12, 0x20003C14, 0x20003C16,..., 0x20003C1E
    float CPU_temp;                     //0x20003C20
    float VCC;                          //0x20003C24
    uint16_t servo_us[SERVOS_NUMBER];   //0x20003C28, 0x20003C2A, 0x20003C2C, 0x20003C2E,..., 0x20003C36
    uint8_t valve_status[SERVOS_NUMBER];//0x20003C38, 0x20003C39, 0x20003C3A, 0x20003C3B,..., 0x20003C3F
    uint32_t time_change_position;      //0x20003C40
    uint32_t is_servo_power_on;         //0x20003C44
    float voltage_servo;                //0x20003C48
    float current_servo;                //0x20003C4C
    float power_servo;                  //0x20003C50

} var_t;

typedef struct
{
    common_param_t common;
    uint32_t crc;
    uint32_t size;
    uint16_t servo_close_us[SERVOS_NUMBER];
    uint16_t servo_open_us[SERVOS_NUMBER];
    uint32_t servo_power_time_us;
    uint32_t valve_active_time_us;
    uint16_t ina219_config;
    uint16_t reserve_1;
    float    r_shunt;

} param_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern var_t v;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define VALVE_1_Pin GPIO_PIN_0
#define VALVE_1_GPIO_Port GPIOA
#define VALVE_2_Pin GPIO_PIN_1
#define VALVE_2_GPIO_Port GPIOA
#define VALVE_3_Pin GPIO_PIN_2
#define VALVE_3_GPIO_Port GPIOA
#define VALVE_4_Pin GPIO_PIN_3
#define VALVE_4_GPIO_Port GPIOA
#define VALVE_5_CLOSE_Pin GPIO_PIN_1
#define VALVE_5_CLOSE_GPIO_Port GPIOB
#define VALVE_5_OPEN_Pin GPIO_PIN_2
#define VALVE_5_OPEN_GPIO_Port GPIOB
#define VALVE_6_CLOSE_Pin GPIO_PIN_10
#define VALVE_6_CLOSE_GPIO_Port GPIOB
#define VALVE_6_OPEN_Pin GPIO_PIN_11
#define VALVE_6_OPEN_GPIO_Port GPIOB
#define SERVO_POWER_Pin GPIO_PIN_12
#define SERVO_POWER_GPIO_Port GPIOB
#define SERVO_1_Pin GPIO_PIN_8
#define SERVO_1_GPIO_Port GPIOA
#define SERVO_2_Pin GPIO_PIN_9
#define SERVO_2_GPIO_Port GPIOA
#define SERVO_3_Pin GPIO_PIN_10
#define SERVO_3_GPIO_Port GPIOA
#define SERVO_4_Pin GPIO_PIN_11
#define SERVO_4_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
