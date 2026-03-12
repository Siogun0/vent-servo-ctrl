/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const meta_data_t __attribute__((section(".boot_vers_sec"))) bootloader_version;
const meta_data_t __attribute__((section(".app_vers_sec"))) application_version =
{
    .version = VERSION_FULL_STRING,
    .build_date = BUILD_DATE,
    //.target_cpu = CPU,
    .crc = UINT32_MAX
};


               __attribute__((section(".calib_ram_sec")))   param_t param;
volatile const __attribute__((section(".calib_flash_sec"))) param_t param_flash;
const param_t param_def =
{
    .common.crc = 0,
    .common.size = sizeof(common_param_t),
    .common.xcp_canid_rx = XCP_BASE_ID,
    .common.xcp_canid_tx = XCP_BASE_ID + 1,
    .common.uds_canid_rx = 0,
    .common.uds_canid_func = 0,
    .common.uds_canid_tx = 0,
    .common.ip_mac = {},
    .common.ip_v4 = {},
    .common.ip_port_xcp = 0,
    .common.id = 0,

    .crc = 0,
    .size = sizeof(param_t) - sizeof(common_param_t),
    .servo_close_us = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
    .servo_open_us = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000},
};

uint32_t xcp_base_id = XCP_BASE_ID;
uint16_t config_number = 0;
extern uint32_t xcp_can_is_active;

extern uint32_t _start_calib_ram[];
extern uint32_t _end_calib_ram[];
extern t_xcp_download_cb update_values;

volatile t_can_node_valves_bus0_input can_in;
volatile t_can_node_valves_bus0_output can_out;

var_t __attribute__((section(".var_ram_sec"))) v;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void try_read_xcp_id(void);
void load_param(void);
void update_param_crc(uint32_t address);

void deinit_perif(void) {};

uint32_t get_servo_us(uint32_t position, uint32_t closed, uint32_t opened);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void can_node_valve_status_cb(uint32_t id, uint64_t msg, uint32_t dlc)
{
    can_out.VALVE_STATUS.CPU_TEMP = roundf(v.CPU_temp);
    can_out.VALVE_STATUS.V_3V3 = v.VCC;
}

void can_node_ctrl_to_valve_cb(uint32_t id, uint64_t msg, uint32_t dlc)
{
    v.servo_us[0] = get_servo_us(can_in.CTRL_TO_VALVE.VALVE_1_REQ, param.servo_close_us[0], param.servo_open_us[0]);
    v.servo_us[1] = get_servo_us(can_in.CTRL_TO_VALVE.VALVE_2_REQ, param.servo_close_us[1], param.servo_open_us[1]);
    v.servo_us[2] = get_servo_us(can_in.CTRL_TO_VALVE.VALVE_3_REQ, param.servo_close_us[2], param.servo_open_us[2]);
    v.servo_us[3] = get_servo_us(can_in.CTRL_TO_VALVE.VALVE_4_REQ, param.servo_close_us[3], param.servo_open_us[3]);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t data[8];

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, data) != HAL_OK) return;

    platform_can_msg_recieve(0, &rx_header, data);

}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_RxFifo0MsgPendingCallback(hcan);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    return;
}

uint32_t get_servo_us(uint32_t position, uint32_t closed, uint32_t opened)
{
    uint32_t range = 0;
    uint32_t shift = closed;
    if(closed <= opened)
    {
        range = opened - closed;
    }
    else
    {
        range = closed - opened;
        shift = opened;
    }
    return shift + position * range / 100;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    memset(&v, 0 , sizeof(v));
    memset(&_start_calib_ram, 0xFF , SECTOR_SIZE);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN_Init();
  MX_ADC1_Init();
  MX_CRC_Init();
  MX_IWDG_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  platform_can_init();

  load_param();
  try_read_xcp_id();

  xcp_can_init(0, 0, xcp_base_id);

  update_values = update_param_crc;

  can_node_valves_bus0_init(xcp_used_mbxs(), 0, 0, &can_out, &can_in);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//  ADC_Start();

  __enable_irq();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    xcp_can_poll();
    can_node_valves_bus0_rx(&can_in);


    can_node_valves_bus0_tx(&can_out);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void try_read_xcp_id(void)
{
    if(param.common.xcp_canid_tx == param.common.xcp_canid_rx + 1 && param.common.xcp_canid_rx != 0xFFFFFFFF)
    {
        xcp_base_id = param.common.xcp_canid_rx;
    }
    else
    {
        xcp_base_id += config_number * 2;
    }
}

void load_param(void)
{
    static_assert(sizeof(common_param_t) == COMMON_PARAM_SIZE, "Size of common param must be 64");
    uint32_t crc = 0;

    // Load defaul params
    memcpy(&param, &param_def, sizeof(param_t));

    //Check saved common.params and load
    if(param_flash.common.size <= (SECTORS_FOR_PARAM * SECTOR_SIZE) && (param_flash.common.size != 0) && (param_flash.common.size % 4 == 0))
    {
        crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&param_flash.common.size, param_flash.common.size / 4 - 1);
        if(param_flash.common.crc == crc)
        {
            if(param_flash.size <= (SECTORS_FOR_PARAM * SECTOR_SIZE) && (param_flash.size != 0) && (param_flash.size % 4 == 0))
            {
                crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&param_flash.size, param_flash.size / 4 - 1);
                if(param_flash.crc == crc)
                {
                    memcpy(&param, (const void *)&param_flash, param_flash.common.size + param_flash.size);
                }
            }
        }
    }

    //Recalculate CRC
    param.common.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&param.common.size, param.common.size / 4 - 1);
    param.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&param.size, param.size / 4 - 1);
}

void update_param_crc(uint32_t address)
{
    if(address >= (uint32_t)&param.common && address < (uint32_t)&param.common + sizeof(common_param_t))
    {
        param.common.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&param.common.size, param.common.size / 4 - 1);
    }

    if(address >= (uint32_t)&param.crc && address < (uint32_t)&param + sizeof(param_t))
    {
        param.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&param.size, param.size / 4 - 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
