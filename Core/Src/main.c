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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef union
{
    float f;
    uint8_t b[4];
} FloatBytes;

typedef struct
{
	FloatBytes V1;
	uint8_t r_V1;
	FloatBytes s_V1;
	FloatBytes V2;
	uint8_t r_V2;
	FloatBytes s_V2;
	FloatBytes V3;
	uint8_t r_V3;
	FloatBytes s_V3;
} Voltages;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SERIAL_BUFFER_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
FDCAN_TxHeaderTypeDef   TxHeader;
FDCAN_RxHeaderTypeDef   RxHeader;
uint8_t TxData[12];
uint8_t RxData[4];

Voltages volts;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
static void FDCAN_Config();
void Serial_Print(UART_HandleTypeDef *huart, const char *fmt, ...);
void Run_Balancing();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_FDCAN2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  FDCAN_Config();
  Serial_Print(&huart1, "Hello World");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(volts.r_V1 == 1 || volts.r_V2 == 1 || volts.r_V3 == 1)
	  {
		  Run_Balancing();
	  }
	  TxHeader.Identifier = 0x02;
	  TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;
	  TxHeader.DataLength = FDCAN_DLC_BYTES_4;
	  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, NULL)!= HAL_OK)
	  {
		Error_Handler();
	  }
	  TxHeader.Identifier = 0x03;
	  TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;
	  TxHeader.DataLength = FDCAN_DLC_BYTES_4;
	  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, NULL)!= HAL_OK)
	  {
		Error_Handler();
	  }
	  TxHeader.Identifier = 0x04;
	  TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;
	  TxHeader.DataLength = FDCAN_DLC_BYTES_4;
	  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, NULL)!= HAL_OK)
	  {
		Error_Handler();
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 8;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 2;
  hfdcan2.Init.NominalTimeSeg2 = 1;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 1;
  hfdcan2.Init.DataTimeSeg2 = 1;
  hfdcan2.Init.StdFiltersNbr = 1;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void FDCAN_Config()
{
	FDCAN_FilterTypeDef sFilterConfig;

	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = 0x10;
	sFilterConfig.FilterID2 = 0x20;
	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
	{
	  /* Filter configuration Error */
	  Error_Handler();
	}

	if(HAL_FDCAN_Start(&hfdcan2)!= HAL_OK)
	{
	 Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		Error_Handler();
	}

	// Configure TX Header for FDCAN1
	TxHeader.Identifier = 0x01;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_12;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		/* Retreive Rx messages from RX FIFO1 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
		{
			/* Reception Error */
			Error_Handler();
		}

		if(RxHeader.Identifier == 0x10)
		{
			volts.V1.b[0] = RxData[0];
			volts.V1.b[1] = RxData[1];
			volts.V1.b[2] = RxData[2];
			volts.V1.b[3] = RxData[3];
			volts.r_V1 = 1;
		}
		else if(RxHeader.Identifier == 0x11)
		{
			volts.V2.b[0] = RxData[0];
			volts.V2.b[1] = RxData[1];
			volts.V2.b[2] = RxData[2];
			volts.V2.b[3] = RxData[3];
			volts.r_V2 = 1;
		}
		else if(RxHeader.Identifier == 0x12)
		{
			volts.V3.b[0] = RxData[0];
			volts.V3.b[1] = RxData[1];
			volts.V3.b[2] = RxData[2];
			volts.V3.b[3] = RxData[3];
			volts.r_V3 = 1;
		}
	}
}

void Run_Balancing()
{
	float sum = volts.V1.f + volts.V2.f + volts.V3.f;
	float mean = sum/3;
	float dif[3];
	float k[3];

	dif[0] = mean - volts.V1.f;
	dif[1] = mean - volts.V2.f;
	dif[2] = mean - volts.V3.f;
	// ===============================
	// Encontrar maior valor em módulo
	// ===============================

	float maior = fabsf(dif[0]);
	int indice_maior = 0;

	if(fabsf(dif[1]) > maior)
	{
	    maior = fabsf(dif[1]);
	    indice_maior = 1;
	}

	if(fabsf(dif[2]) > maior)
	{
	    maior = fabsf(dif[2]);
	    indice_maior = 2;
	}

	// ===============================
	// Calcular k[]
	// ===============================

	for(int i = 0; i < 3; i++)
	{
	    if(i == indice_maior)
	    {

	        k[i] = (dif[indice_maior]>0)?-4.0f:4.0f;
	    }
	    else
	    {
	        k[i] = dif[i] * 4.0f / dif[indice_maior];
	    }
	}

	volts.s_V1.f = (fabs(dif[0])>0.01)?k[0]:dif[0]*-5;
	volts.s_V2.f = (fabs(dif[1])>0.01)?k[1]:dif[1]*-5;
	volts.s_V3.f = (fabs(dif[2])>0.01)?k[2]:dif[2]*-5;

	TxData[0] = volts.s_V1.b[0];
	TxData[1] = volts.s_V1.b[1];
	TxData[2] = volts.s_V1.b[2];
	TxData[3] = volts.s_V1.b[3];
	TxData[4] = volts.s_V2.b[0];
	TxData[5] = volts.s_V2.b[1];
	TxData[6] = volts.s_V2.b[2];
	TxData[7] = volts.s_V2.b[3];
	TxData[8] = volts.s_V3.b[0];
	TxData[9] = volts.s_V3.b[1];
	TxData[10] = volts.s_V3.b[2];
	TxData[11] = volts.s_V3.b[3];


	TxHeader.Identifier = 0x01;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_12;
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, TxData)!= HAL_OK)
	{
		Error_Handler();
	}

	volts.r_V1 = 0;
	volts.r_V2 = 0;
	volts.r_V3 = 0;
}

void Serial_Print(UART_HandleTypeDef *huart, const char *fmt, ...)
{
    char buffer[SERIAL_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);

    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);

    if (len > 0)
    {
        if (len > sizeof(buffer))
            len = sizeof(buffer);

        HAL_UART_Transmit(huart, (uint8_t *)buffer, len, HAL_MAX_DELAY);
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
