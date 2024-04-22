#include "driver_motorComm.h"
#include "pid.h"
#include "Motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups


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
	float targetAngle = 0;
	MotorPIDInit();
	while(1){
		xQueuePeek(TargetAngleQueueHandle, &targetAngle, 0);
		SendMessage2Motor(cascade_loop(targetAngle, g_currentMotorInf.angle, g_currentMotorInf.speed), motorID);
		//printf("%f, %f, %f\n", angle, baseAngle, targetAngle);
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
