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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SUCCESS 0
#define FAILURE 1

#define NO_ACC	0
#define ACC		1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

uint32_t RXID;
uint8_t RXDLC, RXdata[8];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
/* USER CODE BEGIN PFP */

void CAN_initReceptionFilters(uint8_t Filter_Num, uint16_t STD_ID, uint16_t STD_Mask);
uint8_t CAN_sendAsenqMsg(uint32_t ID, uint8_t DLC , uint8_t* data);
void CAN_receiveAsenqMsg(uint32_t* ID, uint8_t* DLC , uint8_t* data);

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

	uint8_t ACC_State = NO_ACC;

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
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */

  CAN_initReceptionFilters(0, 0x03AB, 0x7FF);

  if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK);
  	  Error_Handler();

  if(HAL_CAN_Start(&hcan) != HAL_OK)
		Error_Handler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  while(CAN_sendAsenqMsg(0x030, 1 , &ACC_State) != SUCCESS);
	  HAL_Delay(1000);
	  ACC_State ^=1;


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}





void CAN_initReceptionFilters(uint8_t Filter_Num, uint16_t STD_ID, uint16_t STD_Mask)
{
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterMode  	   	   = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale	   	   = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation 	   = CAN_FILTER_ENABLE;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterBank	 	   = Filter_Num;

	sFilterConfig.FilterIdLow	   	   = 0;
	sFilterConfig.FilterIdHigh 	       = STD_ID << 5;

	sFilterConfig.FilterMaskIdLow	   = 0;
	sFilterConfig.FilterMaskIdHigh	   = STD_Mask <<5 ;

	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
}

uint8_t CAN_sendAsenqMsg(uint32_t ID, uint8_t DLC , uint8_t* data)
{
	CAN_TxHeaderTypeDef pHeader;
	uint32_t TxMailbox;

	pHeader.DLC   = DLC;
	pHeader.StdId = ID;
	pHeader.IDE   = CAN_ID_STD;
	pHeader.RTR   = CAN_RTR_DATA;

	// check if there is a free space to send.
	if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 0)
	{

		if(HAL_CAN_AddTxMessage(&hcan, &pHeader, data, &TxMailbox) != HAL_OK)
			Error_Handler();

		return SUCCESS;
	}
	else
		return FAILURE;
}

void CAN_receiveAsenqMsg(uint32_t* ID, uint8_t* DLC , uint8_t* data)
{
	CAN_RxHeaderTypeDef pHeader;

	if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &pHeader, data)!= HAL_OK)
		Error_Handler();

	*DLC = pHeader.DLC;
	*ID  = pHeader.StdId;

}

void HAL_CAN_RxFIFO0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_receiveAsenqMsg(&RXID, &RXDLC , RXdata);
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 1;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
