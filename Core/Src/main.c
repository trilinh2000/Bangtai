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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	PROGRAM_STOPPED,
	PROGRAM_RUNNING,
}ProgramState;
typedef enum {
	Motor_load,
	Motor_unload,
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
/* Definitions for buttonTask */
osThreadId_t buttonTaskHandle;
const osThreadAttr_t buttonTask_attributes = {
  .name = "buttonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* USER CODE BEGIN PV */
ProgramState programState=PROGRAM_RUNNING;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void *argument);
void StartTask02(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t req=0;
uint16_t tag=0;

void Motor_driver(Motor status)
{
	switch(status)
	{
		case Motor_load:
			HAL_GPIO_WritePin(Y00_GPIO_Port, Y00_Pin, GPIO_PIN_SET);//Motor Load
			HAL_GPIO_WritePin(Y01_GPIO_Port, Y01_Pin, GPIO_PIN_RESET);
			break;
		case Motor_unload:
			HAL_GPIO_WritePin(Y00_GPIO_Port, Y00_Pin, GPIO_PIN_SET);//Motor UnLoad
			HAL_GPIO_WritePin(Y01_GPIO_Port, Y01_Pin, GPIO_PIN_SET);
			break;
		default:
			HAL_GPIO_WritePin(Y00_GPIO_Port, Y00_Pin, GPIO_PIN_RESET);//Motor Stop
			HAL_GPIO_WritePin(Y01_GPIO_Port, Y01_Pin, GPIO_PIN_RESET);
			break;
	}


}
void Program_Run_Load(void)
{
	if(HAL_GPIO_ReadPin(X04_GPIO_Port, X04_Pin)==1)//cb1
	{
		tag=1;
	}
	if(tag==1)
	{
		if(HAL_GPIO_ReadPin(X07_GPIO_Port, X07_Pin)==0)//red sensor3
		{
			HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_SET);//write sensor 3
			Motor_driver(Motor_load);
			if(HAL_GPIO_ReadPin(X04_GPIO_Port, X04_Pin)==0)//cb1=0
			{
				Motor_driver(Motor_stop);
				HAL_GPIO_WritePin(Y02_GPIO_Port, Y02_Pin, GPIO_PIN_SET);//wirte sensor 1
				req=0;
				tag=0;
				HAL_Delay(500);
				HAL_GPIO_WritePin(Y02_GPIO_Port, Y02_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_RESET);
			}

		}
		else {
			Motor_driver(Motor_stop);
		}
	}

}
void Program_Run_UnLoad(void)
{
	if(HAL_GPIO_ReadPin(X04_GPIO_Port, X04_Pin)==0)//co hang cb 2
		{
			tag=1;
		}
		if(tag==1)
		{
			if(HAL_GPIO_ReadPin(X07_GPIO_Port, X07_Pin)==0)
			{
				HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_SET);
				Motor_driver(Motor_unload);
				if(HAL_GPIO_ReadPin(X05_GPIO_Port, X05_Pin)==0)
				{
					Motor_driver(Motor_stop);
					HAL_GPIO_WritePin(Y02_GPIO_Port, Y02_Pin, GPIO_PIN_SET);
					req=0;
					tag=0;
					HAL_Delay(500);
					HAL_GPIO_WritePin(Y02_GPIO_Port, Y02_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_RESET);
				}
			}
			else
			{
				Motor_driver(Motor_stop);
			}

		}

}
void Program_Stop(void)
{
		Motor_driver(Motor_stop);
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
  HAL_GPIO_WritePin(Y02_GPIO_Port, Y02_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Y03_GPIO_Port, Y03_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of buttonTask */
  buttonTaskHandle = osThreadNew(StartDefaultTask, NULL, &buttonTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  HAL_GPIO_WritePin(GPIOB, Y02_Pin|Y03_Pin|Y06_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Y05_Pin|Y00_Pin|Y01_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Y04_GPIO_Port, Y04_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : X00_Pin X01_Pin X05_Pin */
  GPIO_InitStruct.Pin = X00_Pin|X01_Pin|X05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Y02_Pin Y03_Pin Y06_Pin */
  GPIO_InitStruct.Pin = Y02_Pin|Y03_Pin|Y06_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : X10_Pin X06_Pin X07_Pin */
  GPIO_InitStruct.Pin = X10_Pin|X06_Pin|X07_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : X02_Pin */
  GPIO_InitStruct.Pin = X02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
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

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the buttonTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	 if(HAL_GPIO_ReadPin(X00_GPIO_Port, X00_Pin)==0)
	 {
		 programState=PROGRAM_STOPPED;
		 Program_Stop();
	 }
	 if(HAL_GPIO_ReadPin(X00_GPIO_Port, X00_Pin)==1&&HAL_GPIO_ReadPin(X01_GPIO_Port, X01_Pin)==0)
	 {
		 programState=PROGRAM_RUNNING;
	 }
	 if(HAL_GPIO_ReadPin(X02_GPIO_Port, X02_Pin)==0)
	 {
		 programState=PROGRAM_STOPPED;
		 Program_Stop();
	 }
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
	  if(programState==PROGRAM_RUNNING){
		  if(HAL_GPIO_ReadPin(X06_GPIO_Port, X06_Pin)==1)
			  {
				  Program_Run_Load();
			  }
			  else if(HAL_GPIO_ReadPin(X06_GPIO_Port, X06_Pin)==0){
				  Program_Run_UnLoad();
			  }
	  }
	  else{

	  }

    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
