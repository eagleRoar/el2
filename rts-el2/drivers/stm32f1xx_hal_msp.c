/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : stm32f1xx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>

//PVD
#define PVD_Pin GPIO_PIN_0
#define PVD_GPIO_Port GPIOA

#define CH1_PW_Pin GPIO_PIN_6
#define CH1_PW_GPIO_Port GPIOA
#define CH2_PW_Pin GPIO_PIN_7
#define CH2_PW_GPIO_Port GPIOA

#define CH1_TA_Pin GPIO_PIN_0
#define CH1_TA_GPIO_Port GPIOB
#define CH2_TA_Pin GPIO_PIN_1
#define CH2_TA_GPIO_Port GPIOB
#define CH1_TA_EN_Pin GPIO_PIN_2
#define CH1_TA_EN_GPIO_Port GPIOB
#define CH2_TA_EN_Pin GPIO_PIN_3
#define CH2_TA_EN_GPIO_Port GPIOB

#define DBG_TX_Pin GPIO_PIN_9
#define DBG_TX_GPIO_Port GPIOA
#define DBG_RX_Pin GPIO_PIN_10
#define DBG_RX_GPIO_Port GPIOA

#define SENSOR_TX_Pin GPIO_PIN_10
#define SENSOR_TX_GPIO_Port GPIOB
#define SENSOR_RX_Pin GPIO_PIN_11
#define SENSOR_RX_GPIO_Port GPIOB

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /* System interrupt init*/

    /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
     */
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    /* USER CODE BEGIN MspInit 1 */
    DBGMCU->CR &= ~((uint32_t)1 << 5); //关闭异步跟踪
    AFIO->MAPR = (AFIO->MAPR & ~((uint32_t)0x7 << 24)) | (2 << 24); /*  PA15 PB3 PB4 */
    /* USER CODE END MspInit 1 */
}

/**
 * @brief ADC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hadc->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
         PA0-WKUP     ------> ADC1_IN0
         PA6     ------> ADC1_IN6
         PB0     ------> ADC1_IN8
         */
        GPIO_InitStruct.Pin = PVD_Pin | CH1_PW_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = CH1_TA_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(CH1_TA_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    } else if (hadc->Instance == ADC2) {
        /* USER CODE BEGIN ADC2_MspInit 0 */

        /* USER CODE END ADC2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC2 GPIO Configuration
         PA7     ------> ADC2_IN7
         PB1     ------> ADC2_IN9
         */
        GPIO_InitStruct.Pin = CH2_PW_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(CH2_PW_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = CH2_TA_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(CH2_TA_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC2_MspInit 1 */

        /* USER CODE END ADC2_MspInit 1 */
    }

    HAL_ADCEx_Calibration_Start(hadc); //校验ADC
}

/**
 * @brief ADC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
         PA0-WKUP     ------> ADC1_IN0
         PA6     ------> ADC1_IN6
         PB0     ------> ADC1_IN8
         */
        HAL_GPIO_DeInit(GPIOA, PVD_Pin | CH1_PW_Pin);

        HAL_GPIO_DeInit(CH1_TA_GPIO_Port, CH1_TA_Pin);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    } else if (hadc->Instance == ADC2) {
        /* USER CODE BEGIN ADC2_MspDeInit 0 */

        /* USER CODE END ADC2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC2_CLK_DISABLE();

        /**ADC2 GPIO Configuration
         PA7     ------> ADC2_IN7
         PB1     ------> ADC2_IN9
         */
        HAL_GPIO_DeInit(CH2_PW_GPIO_Port, CH2_PW_Pin);

        HAL_GPIO_DeInit(CH2_TA_GPIO_Port, CH2_TA_Pin);

        /* USER CODE BEGIN ADC2_MspDeInit 1 */

        /* USER CODE END ADC2_MspDeInit 1 */
    }
}

/**
 * @brief CRC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
    if (hcrc->Instance == CRC) {
        /* USER CODE BEGIN CRC_MspInit 0 */

        /* USER CODE END CRC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CRC_CLK_ENABLE();
        /* USER CODE BEGIN CRC_MspInit 1 */

        /* USER CODE END CRC_MspInit 1 */
    }
}

/**
 * @brief CRC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc)
{
    if (hcrc->Instance == CRC) {
        /* USER CODE BEGIN CRC_MspDeInit 0 */

        /* USER CODE END CRC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CRC_CLK_DISABLE();
        /* USER CODE BEGIN CRC_MspDeInit 1 */

        /* USER CODE END CRC_MspDeInit 1 */
    }
}

/**
 * @brief DAC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hdac->Instance == DAC) {
        /* USER CODE BEGIN DAC_MspInit 0 */

        /* USER CODE END DAC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_DAC_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**DAC GPIO Configuration
         PA4     ------> DAC_OUT1
         PA5     ------> DAC_OUT2
         */
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN DAC_MspInit 1 */

        /* USER CODE END DAC_MspInit 1 */
    }
}

/**
 * @brief DAC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
    if (hdac->Instance == DAC) {
        /* USER CODE BEGIN DAC_MspDeInit 0 */

        /* USER CODE END DAC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DAC_CLK_DISABLE();

        /**DAC GPIO Configuration
         PA4     ------> DAC_OUT1
         PA5     ------> DAC_OUT2
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_5);

        /* USER CODE BEGIN DAC_MspDeInit 1 */

        /* USER CODE END DAC_MspDeInit 1 */
    }
}

/**
 * @brief RTC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    if (hrtc->Instance == RTC) {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */
        HAL_PWR_EnableBkUpAccess();
        /* Enable BKP CLK enable for backup registers */
        __HAL_RCC_BKP_CLK_ENABLE();
        /* Peripheral clock enable */
        __HAL_RCC_RTC_ENABLE();
        /* RTC interrupt Init */
        HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(RTC_IRQn);
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
    }
}

/**
 * @brief RTC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
    if (hrtc->Instance == RTC) {
        /* USER CODE BEGIN RTC_MspDeInit 0 */

        /* USER CODE END RTC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE();

        /* RTC interrupt DeInit */
        HAL_NVIC_DisableIRQ(RTC_IRQn);
        /* USER CODE BEGIN RTC_MspDeInit 1 */

        /* USER CODE END RTC_MspDeInit 1 */
    }
}

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (huart->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
         PA9     ------> USART1_TX
         PA10     ------> USART1_RX
         */
        GPIO_InitStruct.Pin = DBG_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(DBG_TX_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = DBG_RX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DBG_RX_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    } else if (huart->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART3 GPIO Configuration
         PB10     ------> USART3_TX
         PB11     ------> USART3_RX
         */
        GPIO_InitStruct.Pin = SENSOR_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SENSOR_TX_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = SENSOR_RX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(SENSOR_RX_GPIO_Port, &GPIO_InitStruct);

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }
}

/**
 * @brief UART MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
         PA9     ------> USART1_TX
         PA10     ------> USART1_RX
         */
        HAL_GPIO_DeInit(GPIOA, DBG_TX_Pin | DBG_RX_Pin);

        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    } else if (huart->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
         PB10     ------> USART3_TX
         PB11     ------> USART3_RX
         */
        HAL_GPIO_DeInit(GPIOB, SENSOR_TX_Pin | SENSOR_RX_Pin);

        /* USART3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
