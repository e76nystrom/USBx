/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "stm32h7xx_hal_def.h"
#if 0
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#endif

#include "main.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void usbClock_Config(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

extern void tinyMain(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
#if CUBE_INIT_CLK == 0
 tinyMain();
#else

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
 usbClock_Config();


 tinyMain();
#endif
#if 0
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 while (1)
 {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
 }
#endif
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 8;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void usbClock_Config(void)
{
 RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

 PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
 PeriphClkInitStruct.PLL3.PLL3M = 1;
 PeriphClkInitStruct.PLL3.PLL3N = 24;
 PeriphClkInitStruct.PLL3.PLL3P = 2;
 PeriphClkInitStruct.PLL3.PLL3Q = 4;
 PeriphClkInitStruct.PLL3.PLL3R = 2;
 PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
 PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
 PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;

 if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
  Error_Handler();
 }
}

//--------------------------------------------------------------------+
// RCC Clock
//--------------------------------------------------------------------+

void board_stm32h7_clock_init(void)
{
 RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
 RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };

 /* The PWR block is always enabled on the H7 series- there is no clock
    enable. For now, use the default VOS3 scale mode (lowest) and limit clock
    frequencies to avoid potential current draw problems from bus
    power when using the max clock speeds throughout the chip. */

 /* Enable HSE Oscillator and activate PLL1 with HSE as source */
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
 RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
//  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
 RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
 RCC_OscInitStruct.PLL.PLLM = HSE_VALUE/1000000;
 RCC_OscInitStruct.PLL.PLLN = 336;
 RCC_OscInitStruct.PLL.PLLP = 2;
 RCC_OscInitStruct.PLL.PLLQ = 7;
 RCC_OscInitStruct.PLL.PLLR = 2; /* Unused */
 RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_0;
 RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
 RCC_OscInitStruct.PLL.PLLFRACN = 0;
 HAL_RCC_OscConfig(&RCC_OscInitStruct);

 RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
    RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | \
    RCC_CLOCKTYPE_D3PCLK1);
 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
 RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;

 /* Unlike on the STM32F4 family, it appears the maximum APB frequencies are
    device-dependent- 120 MHz for this board according to Figure 2 of
    the datasheet. Dividing by half will be safe for now. */
 RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
 RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
 RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

 /* 4 wait states required for 168MHz and VOS3. */
 HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

 /* Like on F4, on H7, USB's actual peripheral clock and bus clock are
    separate. However, the main system PLL (PLL1) doesn't have a direct
    connection to the USB peripheral clock to generate 48 MHz, so we do this
    dance. This will connect PLL1's Q output to the USB peripheral clock. */
 RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = { 0 };

 RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
 RCC_PeriphCLKInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
 HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
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

#ifdef  USE_FULL_ASSERT
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
