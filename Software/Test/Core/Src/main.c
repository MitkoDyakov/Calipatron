/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ssd1306_fonts.h"
#include "usbd_cdc_if.h"
#include "notchFilter.h"
#include <string.h>
#include <stdio.h>

#define ADC_BUF_LEN (6072u)
volatile uint16_t adc_buf[ADC_BUF_LEN];

#define DISPLAY_ADDRESS_A (0x3Du)
#define DISPLAY_ADDRESS_B (0x3Cu) 

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;
//extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc1;

volatile uint8_t uartReadyFlag = 1;
volatile uint8_t adcReadyFlag = 1;
volatile uint16_t currentPinState = 0;
volatile uint8_t isAdcDmaStarted = 0;
volatile uint8_t startADC = 0;

#define PEAK (116u)
#define NUMBER_OF_PUN_STATES  (128u)
uint32_t pinState[NUMBER_OF_PUN_STATES] = {0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010100000000001010101, 0b00000000101010100000000001010101, 0b00000000010101010000000010101010, 0b00000000010101010000000010101010, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101010010000000001010110, 0b00000000101010010000000001010110, 0b00000000010101100000000010101001, 0b00000000010101100000000010101001, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000101001010000000001011010, 0b00000000101001010000000001011010, 0b00000000010110100000000010100101, 0b00000000010110100000000010100101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010, 0b00000000011010100000000010010101, 0b00000000011010100000000010010101, 0b00000000100101010000000001101010, 0b00000000100101010000000001101010};
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// EXTI Line9 External Interrupt ISR Handler CallBackFun
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_14) // If The INT Source Is EXTI Line9 (A9 Pin)
  {
    if(1 == adcReadyFlag && 1 == uartReadyFlag)
    {
      startADC = 1; 
      adcReadyFlag = 0;
    }
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  adcReadyFlag = 1;
  uartReadyFlag = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        GPIOA->BSRR = pinState[currentPinState];      
        currentPinState++;
        if(NUMBER_OF_PUN_STATES == currentPinState)
        {
          if(1 == startADC)
          {
            HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADC_BUF_LEN);
            startADC = 0;
          }
          currentPinState = 0;
        }
        
        if(PEAK == currentPinState)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        }
        
        if((PEAK+5) == currentPinState)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        }
    }
}

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
    
  HAL_StatusTypeDef ret;

  TIM2->CCR1 = 1;  
  HAL_TIM_Base_Start_IT(&htim2);  
   
//  ret = HAL_I2C_IsDeviceReady(&hi2c1, DISPLAY_ADDRESS_B << 1, 5, HAL_MAX_DELAY);
//  
//  ssd1306_Init();
//  ssd1306_Fill(Black);
//  ssd1306_SetCursor(2, 2);
//  ssd1306_WriteString("Oops!", Font_16x26, White);
//  ssd1306_UpdateScreen();
  uint8_t msg[5] = {0};
  uint16_t i=0;
  
  HAL_ADCEx_Calibration_Start(&hadc1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(0 == uartReadyFlag)
    {
        for(i=0; i<ADC_BUF_LEN; i++)
        {
            memset(msg, 0, 5);
            sprintf(msg, "%d\r", adc_buf[i]);
            while(USBD_BUSY == CDC_Transmit_FS(msg, 5));
        }
        uartReadyFlag = 1;
        startADC = 0;
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
