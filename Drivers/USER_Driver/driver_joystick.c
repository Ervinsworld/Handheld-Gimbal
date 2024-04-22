#include "adc.h"
#include "gpio.h"
#include "driver_joystick.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups

static uint32_t ADC_Buffer[100];//���ڴ���ADC��DMAת�˵����ݣ�����100
buttonState g_ButtonState = free;

//static buttonState g_ButtonState;
extern EventGroupHandle_t UIResponseEvent;
//extern QueueHandle_t ButtonQueueHandle;
/**********************************************************************
 * �������ƣ� JoyStickInit
 * ���������� ҡ�˳�ʼ��
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/3/14	     V1.0	  Ervin	      ����
 ***********************************************************************/
void JoyStickInit(){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Buffer, 100);
}

/**********************************************************************
 * �������ƣ� JoyStickValueCal
 * ���������� ҡ��ֵ���㣬����ÿ���ᣬ��Buffer�е������ۼ�50��ȡƽ��ֵ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� struct JoystickerValue
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/3/14	     V1.0	  Ervin	      ����
 ***********************************************************************/

joystickValue JoyStickValueCal(){
	uint8_t i;
	uint32_t x, y;
	joystickValue value;
	for(i = 0,x =0,y=0; i < 100;)
	{
		x += ADC_Buffer[i++];
		y += ADC_Buffer[i++];
	}
	x /= 50;
	y /= 50;
	//����ҡ�˰�װ��ʽ��x, y�Ե�
	value.xValue = y;
	value.yValue = x;
	return value;
}	

/**********************************************************************
 * �������ƣ� JoystickButton_IRQ_Callback
 * ���������� ҡ�˰������жϻص�����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸����ڣ�      �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 *  2024/4/7	     V1.0	  Ervin	      ����
 ***********************************************************************/

void JoystickButton_IRQ_Callback(void){
	if(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin)==GPIO_PIN_SET){
		g_ButtonState = free;
		//xQueueOverwriteFromISR(ButtonQueueHandle, &g_ButtonState, NULL);
	}	
	else{
		g_ButtonState = pressed;
		//xQueueOverwriteFromISR(ButtonQueueHandle, &g_ButtonState, NULL);
	}
		
};


void Joystick_Test(void *params){
	joystickValue value;
	int buttonValue;
	while(1){
		value = JoyStickValueCal();
		printf("xValue = %d, yValue = %d, ButtonValue = %d\n", value.xValue, value.yValue, g_ButtonState);
	}
}
