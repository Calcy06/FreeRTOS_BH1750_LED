/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "bh1750.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// 软件定时器句柄
osTimerId_t TimerBH1750Handle;

// 任务句柄
osThreadId_t taskBH1750Handle;
osThreadId_t taskLedHandle;

// 队列句柄
osMessageQueueId_t lx_handle;

// // BH1750软件定时器回调函数
// void TimerBH1750Callback(void *argument)
// {
//     BH1750_get_lumen(&bh1750_dev);
// }

// 任务1
void Task_bh1750(void *argument)
{
    bh1750_dev_t *dev = (bh1750_dev_t *)argument;
    TickType_t xLastWake = xTaskGetTickCount();    // 记录当前时刻
    const TickType_t xPeriod = pdMS_TO_TICKS(500); // 周期500ms

    uint16_t lx = 0;

    while (1)
    {
        vTaskDelayUntil(&xLastWake, xPeriod); // 精确 500ms 周期
        BH1750_get_lumen(dev);
        if (osMessageQueuePut(lx_handle, &(dev->value), NULL, NULL) != osOK)
        {
            printf("QueuePut Failed\r\n");
        }
    }
}

// 任务2
void Task_led(void *argument)
{
    // bh1750_dev_t *dev = (bh1750_dev_t *)argument;
    uint16_t lx;
    uint16_t ret;

    while (1)
    {

        // printf("lumen:%d\r\n", dev->value);
        ret = osMessageQueueGet(lx_handle, &lx, NULL, osWaitForever);
        if (ret == osOK)
        {
            printf("lumen:%u\r\n", lx);
            if (lx < 3)
            {
                HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);
            }
        }
        else
        {
            printf("QueueGet failed\r\n");
        }
        osDelay(pdMS_TO_TICKS(200));
    }
}
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
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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
    lx_handle = osMessageQueueNew(4, sizeof(uint16_t), NULL);
    configASSERT(lx_handle != NULL);
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    const osThreadAttr_t taskBH1750Attr = {
        .name = "TaskBH1750",
        .stack_size = 128 * 4,
        .priority = (osPriority_t)osPriorityAboveNormal,
    };
    taskBH1750Handle = osThreadNew(Task_bh1750, &bh1750_dev, &taskBH1750Attr);
    configASSERT(taskBH1750Handle != NULL);

    const osThreadAttr_t taskLedAttr = {
        .name = "TaskLED",
        .stack_size = 128 * 4,
        .priority = (osPriority_t)osPriorityNormal,
    };
    taskLedHandle = osThreadNew(Task_led, &bh1750_dev, &taskLedAttr);
    configASSERT(taskLedHandle != NULL);
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
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
