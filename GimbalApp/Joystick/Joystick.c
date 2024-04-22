#include "driver_joystick.h"
#include "page.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups

/**********************************************************************
 * �������ƣ� Joystick_Task
 * ���������� FreeRTOSҡ������,ÿ100tickȥDMA�����м���һ��ҡ��ֵ
 * ��������� params
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/17	     V1.0	  Ervin	      ����
 ***********************************************************************/
void Joystick_Task(void *params){
	joystickValue value;
	extern EventGroupHandle_t UIResponseEvent;
	extern buttonState g_ButtonState;
	while(1){
		value = JoyStickValueCal();
		if(value.xValue<500)
			xEventGroupSetBits(UIResponseEvent, 1<<0);
		else if(value.xValue>3500)
			xEventGroupSetBits(UIResponseEvent, 1<<1);
		else if(value.yValue>3500)
			xEventGroupSetBits(UIResponseEvent, 1<<2);
		else if(value.yValue<500)
			xEventGroupSetBits(UIResponseEvent, 1<<3);
		if(g_ButtonState == pressed)
			xEventGroupSetBits(UIResponseEvent, 1<<4);
		//printf("PageID:%d\n", getCurrentpageId());
		vTaskDelay(100);
	}
}
