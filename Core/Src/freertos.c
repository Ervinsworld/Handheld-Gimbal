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
#include <stdio.h>
#include "driver_motorComm.h"
//#include "driver_mpu6050.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "driver_led.h"
#include "driver_joystick.h"
#include "driver_oled.h"
#include "UI.h"
#include "page.h"
#include "joystick.h"
#include "Motor.h"
/** User FreeRTOS  **/
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
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
TaskHandle_t MotorPidTaskHandle;

//QueueHandle_t MotorInfQueueHandle;

QueueHandle_t TargetAngleQueueHandle;
QueueHandle_t AngleDiffQueueHandle;
//QueueHandle_t MotorRawQueueHandle;

//QueueHandle_t ButtonQueueHandle;

//SemaphoreHandle_t ActionSemaphore;
EventGroupHandle_t UIResponseEvent;//ҡ���¼���Ӧ��event���������Ұ��£�
EventGroupHandle_t UIActionEvent;//UI��ҳ��͵�������������¼�

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
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
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	OLED_Init();//oled��ʼ��
	JoyStickInit();//ҡ�˳�ʼ��
	MotorCommInit();//���ͨѶ��ʼ��
	//MPU_Init();//MPU6050��ʼ��
    //mpu_dmp_init();		//dmp��ʼ��
	printf("Everything is Inited��\r\n");
	
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	//ActionSemaphore = xSemaphoreCreateBinary();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

	//MotorRawQueueHandle = xQueueCreate(1, sizeof(uint8_t)*8);//����CANͨ�Ŵ�����ԭʼ�������֡
	TargetAngleQueueHandle = xQueueCreate(1, sizeof(float));//UIҳ����Ҫ����angle��Ŀ��ֵmailbox
	AngleDiffQueueHandle = xQueueCreate(1, sizeof(float));//���������ս�����ҳ��ʱ��angle���죨����������ҳ��ʱ�ĵ������ת����
	
	//ButtonQueueHandle = xQueueCreate(1, sizeof(int));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  

  xTaskCreate(MotorPid_Task, "MotorPidTask", 128, NULL, osPriorityNormal+2, &MotorPidTaskHandle);
  xTaskCreate(Joystick_Task, "JoystickTask", 128, NULL, osPriorityNormal+1, NULL);
  xTaskCreate(UI_Task, "UITask", 256, NULL, osPriorityNormal+1, NULL);
  xTaskCreate(UIAction_Task, "UIAction_Task", 256, NULL, osPriorityNormal+1, NULL);
  
  /*test_thread*/
  //xTaskCreate(AnglePrint_Task, "test", 128, NULL, osPriorityNormal+1, NULL);
 
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  
  //UI��ҳ��Ӧ�¼���ʹ�õ���λ��0λ���ң�1λ����2λ���£�3λ���ϣ�4λ����ť
  UIResponseEvent = xEventGroupCreate();
  //UI�����¼�ֻ�����λ
  UIActionEvent = xEventGroupCreate(); 
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
	vTaskDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

