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
typedef enum {
	PROGRAM_STOPPED=0,
	PROGRAM_RUNNING
}ProgramState;
typedef enum {
	Motor_run,
	Motor_stop,
}Motor;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ProgramState programState=PROGRAM_STOPPED;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int req=0;
void Motor_driver(Motor status)
{
	if(status==Motor_run)
	{
		HAL_GPIO_WritePin(Y00_GPIO_Port, Y00_Pin, GPIO_PIN_SET);//bat bang tai
		HAL_GPIO_WritePin(Y01_GPIO_Port, Y01_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(Y00_GPIO_Port, Y00_Pin, GPIO_PIN_RESET);//dung bang tai
		HAL_GPIO_WritePin(Y01_GPIO_Port, Y01_Pin, GPIO_PIN_RESET);
	}

}
void Program_Run(void)
{
	if(HAL_GPIO_ReadPin(X04_GPIO_Port,X04_Pin)==0&&req==0)
		{
			if(HAL_GPIO_ReadPin(X03_GPIO_Port,X03_Pin)==1)//k co hang o cam bien 1
			{
				Motor_driver(Motor_run);
			}
			else{
				Motor_driver(Motor_stop);
			}

		}

	else
	{
		if(HAL_GPIO_ReadPin(X03_GPIO_Port,X03_Pin)==0&&req==0)//co hang o cam bien 1
		{
			Motor_driver(Motor_stop);
		}
		else
		{
			HAL_GPIO_WritePin(Y05_GPIO_Port, Y05_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_RESET);
			if(HAL_GPIO_ReadPin(X05_GPIO_Port,X05_Pin)==0)//AGV confirm start tra hang
			{
				req=1;
				HAL_GPIO_WritePin(Y03_GPIO_Port, Y03_Pin, GPIO_PIN_SET);//confrim AGV tra hang
				Motor_driver(Motor_run);
				if(HAL_GPIO_ReadPin(X03_GPIO_Port,X03_Pin)==0&&HAL_GPIO_ReadPin(X04_GPIO_Port,X04_Pin)==1)//co tin hieu cam bien 2 va k co tin hieu cb1
				{
					Motor_driver(Motor_stop);
					req=0;
					HAL_GPIO_WritePin(Y03_GPIO_Port, Y03_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(Y06_GPIO_Port, Y06_Pin, GPIO_PIN_SET);//confirm thanh cong
					HAL_Delay(5000);
					HAL_GPIO_WritePin(Y06_GPIO_Port, Y06_Pin, GPIO_PIN_RESET);
				}
			}
			else if(HAL_GPIO_ReadPin(X07_GPIO_Port,X07_Pin)==0)//agv confirm stop
			{
				Motor_driver(Motor_stop);
			}
		}
	}
}
void Program_Stop(void)
{
	while(HAL_GPIO_ReadPin(X00_GPIO_Port,X00_Pin)==0||HAL_GPIO_ReadPin(X01_GPIO_Port,X01_Pin)==1)//nhan emg va start chua bam
	{
		HAL_GPIO_WritePin(Y03_GPIO_Port, Y03_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_SET);//bang tai con firm cho agv stop
		HAL_GPIO_WritePin(Y05_GPIO_Port, Y05_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Y06_GPIO_Port, Y06_Pin, GPIO_PIN_RESET);

		Motor_driver(Motor_stop);
	}
	HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_SET);//reset stop con firm


}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0)
	{ // Nút EMG
		if (programState == PROGRAM_RUNNING)
		{
				programState = PROGRAM_STOPPED;
				// G�?i hàm dừng chương trình
				Program_Stop();
		}
	}
	else if (GPIO_Pin == GPIO_PIN_1)
	{ // Nút Start
	        if (programState == PROGRAM_STOPPED) {
	            programState = PROGRAM_RUNNING;
	            // G�?i hàm chạy chương trình
//	            Program_Run();
	        }
	}

	else if (GPIO_Pin == GPIO_PIN_6)
	{ // Nút Reset
//	        NVIC_SystemReset(); // Thiết lập lại vi đi�?u khiển STM32
		if (programState == PROGRAM_RUNNING)
		{
				programState = PROGRAM_STOPPED;
				while(HAL_GPIO_ReadPin(X01_GPIO_Port,X01_Pin)==1)//start chua bam
				{
					HAL_GPIO_WritePin(Y03_GPIO_Port, Y03_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_SET);//bang tai confirm cho agv stop
					HAL_GPIO_WritePin(Y05_GPIO_Port, Y05_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(Y06_GPIO_Port, Y06_Pin, GPIO_PIN_RESET);

					Motor_driver(Motor_stop);

				}
				HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_RESET);
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(programState==PROGRAM_RUNNING){
		  Program_Run();
	  }
	  else
	  {
		  Motor_driver(Motor_stop);
	  }

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
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Y02_Pin|Y03_Pin|X10_Pin|Y06_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Y05_Pin|Y00_Pin|Y01_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : X00_Pin X01_Pin */
  GPIO_InitStruct.Pin = X00_Pin|X01_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Y02_Pin Y03_Pin X10_Pin Y06_Pin */
  GPIO_InitStruct.Pin = Y02_Pin|Y03_Pin|X10_Pin|Y06_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : X06_Pin X07_Pin */
  GPIO_InitStruct.Pin = X06_Pin|X07_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : X02_Pin */
  GPIO_InitStruct.Pin = X02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(X02_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : X03_Pin */
  GPIO_InitStruct.Pin = X03_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(X03_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Y05_Pin Y00_Pin Y01_Pin */
  GPIO_InitStruct.Pin = Y05_Pin|Y00_Pin|Y01_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : Y04_Pin */
  GPIO_InitStruct.Pin = Y04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Y04_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : X04_Pin */
  GPIO_InitStruct.Pin = X04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(X04_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : X05_Pin */
  GPIO_InitStruct.Pin = X05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(X05_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
