/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-26     ChenYong     first version
 * 2020-01-08     xiangxistu   add HSI configuration
 */

#include <board.h>
#include <rtthread.h>

#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

void SystemClock_Config(void)
{
//    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
//    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
//    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
//
//    /** Initializes the CPU, AHB and APB busses clocks
//     */
//    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
//    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//    RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
//    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
//    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
//    RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
//    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//            {
//        Error_Handler();
//    }
//    /** Initializes the CPU, AHB and APB busses clocks
//     */
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//            {
//        Error_Handler();
//    }
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC;
//    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
//    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//            {
//        Error_Handler();
//    }
//    /** Configure the Systick interrupt time
//     */
//    __HAL_RCC_PLLI2S_ENABLE();

    RCC_OscInitTypeDef RCC_OscInitStruct =
        { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct =
        { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit =
        { 0 };

    /** Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
    /** Configure the Systick interrupt time 
     */
    __HAL_RCC_PLLI2S_ENABLE();
}

void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_AFIO_CLK_ENABLE()
    ;
    __HAL_RCC_PWR_CLK_ENABLE()
    ;

    /* System interrupt init*/

    /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
     */
    __HAL_AFIO_REMAP_SWJ_NOJTAG()
    ;

    /* USER CODE BEGIN MspInit 1 */
    DBGMCU->CR &= ~((uint32_t) 1 << 5);    //关闭异步跟踪
    AFIO->MAPR = (AFIO->MAPR & ~((uint32_t) 0x7 << 24)) | (2 << 24); /*  PA15 PB3 PB4 */
    /* USER CODE END MspInit 1 */
}

int clock_information(void)
{
    LOG_D("System Clock information");
    LOG_D("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_D("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_D("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_D("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());

    return RT_EOK;
}
INIT_BOARD_EXPORT(clock_information);
