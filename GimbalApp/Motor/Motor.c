#include "driver_motorComm.h"
#include "pid.h"
#include "Motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups

extern MotorInf g_currentMotorInf;

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
	extern QueueHandle_t TargetAngleQueueHandle;
	float targetAngle;
	MotorPIDInit();
	while(1){
		xQueueReceive(TargetAngleQueueHandle, &targetAngle, 0);
		SendMessage2Motor(cascade_loop(targetAngle, g_currentMotorInf.angle, g_currentMotorInf.speed), motorID);
		//printf("%f, %f\n", g_currentMotorInf.angle, g_currentMotorInf.speed);
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}


void Motor_Task(void *params){
	extern EventGroupHandle_t UIResponseEvent;
	vTaskDelay(2000);//����ϵ��ʼ��ʱ��������
	while(1){
		if(g_currentMotorInf.angle<-_PI_4){
			xEventGroupSetBits(UIResponseEvent, 1<<4);
		}
		vTaskDelay(100);
	}
}