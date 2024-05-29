/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "WS2812_task.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Ws2812_task */
osThreadId_t Ws2812_taskHandle;
const osThreadAttr_t Ws2812_task_attributes = {
  .name = "Ws2812_task",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for INS_task */
osThreadId_t INS_taskHandle;
const osThreadAttr_t INS_task_attributes = {
  .name = "INS_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow3,
};
/* Definitions for BUZZER_task */
osThreadId_t BUZZER_taskHandle;
const osThreadAttr_t BUZZER_task_attributes = {
  .name = "BUZZER_task",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for CK_TIMEOUT_TASK */
osThreadId_t CK_TIMEOUT_TASKHandle;
const osThreadAttr_t CK_TIMEOUT_TASK_attributes = {
  .name = "CK_TIMEOUT_TASK",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for CP_TASK */
osThreadId_t CP_TASKHandle;
const osThreadAttr_t CP_TASK_attributes = {
  .name = "CP_TASK",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow3,
};
/* Definitions for GYM_TASK */
osThreadId_t GYM_TASKHandle;
const osThreadAttr_t GYM_TASK_attributes = {
  .name = "GYM_TASK",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow5,
};
/* Definitions for UI_TASK */
osThreadId_t UI_TASKHandle;
const osThreadAttr_t UI_TASK_attributes = {
  .name = "UI_TASK",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for SERVER_TASK */
osThreadId_t SERVER_TASKHandle;
const osThreadAttr_t SERVER_TASK_attributes = {
  .name = "SERVER_TASK",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void ws2812_task(void *argument);
extern void ins_task(void *argument);
extern void buzzer_task(void *argument);
extern void CK_Timeout_task(void *argument);
extern void CP_task(void *argument);
extern void Gym_task(void *argument);
extern void UI_task(void *argument);
extern void Servos_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Ws2812_task */
  Ws2812_taskHandle = osThreadNew(ws2812_task, NULL, &Ws2812_task_attributes);

  /* creation of INS_task */
  INS_taskHandle = osThreadNew(ins_task, NULL, &INS_task_attributes);

  /* creation of BUZZER_task */
  BUZZER_taskHandle = osThreadNew(buzzer_task, NULL, &BUZZER_task_attributes);

  /* creation of CK_TIMEOUT_TASK */
  CK_TIMEOUT_TASKHandle = osThreadNew(CK_Timeout_task, NULL, &CK_TIMEOUT_TASK_attributes);

  /* creation of CP_TASK */
  CP_TASKHandle = osThreadNew(CP_task, NULL, &CP_TASK_attributes);

  /* creation of GYM_TASK */
  GYM_TASKHandle = osThreadNew(Gym_task, NULL, &GYM_TASK_attributes);

  /* creation of UI_TASK */
  UI_TASKHandle = osThreadNew(UI_task, NULL, &UI_TASK_attributes);

  /* creation of SERVER_TASK */
  SERVER_TASKHandle = osThreadNew(Servos_task, NULL, &SERVER_TASK_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

