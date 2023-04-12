//
// Created by Eric on 4/9/2023.
//

#include "stm32h7xx_hal.h"
void Error_Handler(void);

void SystemClock_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

 /** Supply configuration update enable
 */
 HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

 /** Configure the main internal regulator output voltage
 */
 __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

 while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

 /** Initializes the RCC Oscillators according to the specified parameters
 * in the RCC_OscInitTypeDef structure.
 */
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
 RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
 RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
 RCC_OscInitStruct.PLL.PLLM = 1;
 RCC_OscInitStruct.PLL.PLLN = 75;
 RCC_OscInitStruct.PLL.PLLP = 2;
 RCC_OscInitStruct.PLL.PLLQ = 4;
 RCC_OscInitStruct.PLL.PLLR = 2;
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
 RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV4;
 RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
 RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
 {
  Error_Handler();
 }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
 RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

 /** Initializes the peripherals clock
 */
 PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART3;
 PeriphClkInitStruct.PLL3.PLL3M = 1;
 PeriphClkInitStruct.PLL3.PLL3N = 24;
 PeriphClkInitStruct.PLL3.PLL3P = 2;
 PeriphClkInitStruct.PLL3.PLL3Q = 4;
 PeriphClkInitStruct.PLL3.PLL3R = 2;
 PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
 PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
 PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
 PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_PLL3;
 PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
 if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
 {
  Error_Handler();
 }
}

