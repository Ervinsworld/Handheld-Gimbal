#include "driver_motorComm.h"
#include "pid.h"
#include "Motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core

extern QueueHandle_t TargetAngleQueueHandle;
extern QueueHandle_t AngleDiffQueueHandle;
extern SemaphoreHandle_t MotorPidSemaphore;
//extern QueueHandle_t MotorRawQueueHandle;

/**********************************************************************
 * �������ƣ� MotorPid_Task
 * ���������� FreeRTOS�������ʵʱ����pid�㷨��1ms����
 * ��������� params
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/17	     V1.0	  Ervin	      ����
 ***********************************************************************/
void MotorPid_Task(void *params){
	float targetAngle = 0;
	MotorPIDInit();
	xSemaphoreGive(MotorPidSemaphore);//��ʼ״̬���ź���Ϊ1
	while(1){
		xSemaphoreTake(MotorPidSemaphore, portMAX_DELAY);
		xQueuePeek(TargetAngleQueueHandle, &targetAngle, 0);
		SendMessage2Motor(cascade_loop(targetAngle, getMotorAngle(), getMotorSpeed()), motorID);
		//printf("%f, %f, %f\n", angle, baseAngle, targetAngle);
		xSemaphoreGive(MotorPidSemaphore);
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

/**********************************************************************
 * �������ƣ� AnglePrint_Task
 * ���������� ��ӡ����ĸ���Ƕ���Ϣ�����ڵ���
 * ��������� params
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/17	     V1.0	  Ervin	      ����
 ***********************************************************************/
void AnglePrint_Task(void *params){
	float targetAngle = 0;
	float diffAngle = 0;
	while(1){
		//xQueuePeek(MotorRawQueueHandle, &rxData, 0);
		xQueuePeek(TargetAngleQueueHandle, &targetAngle, 0);
		xQueuePeek(AngleDiffQueueHandle, &diffAngle, 0);
		printf("%f, %f, %f\n", getMotorAngle(), diffAngle, targetAngle);//�����Ƕȣ��ǶȲ�ֵ��Ŀ��Ƕ�
		//printf("%f, %f\n", getMotorAngle(), getMotorSpeed());
		vTaskDelay(50);
	}
}