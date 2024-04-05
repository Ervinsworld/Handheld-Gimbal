#include "adc.h"
#include "driver_joystick.h"

static uint32_t ADC_Buffer[100];//���ڴ���ADC��DMAת�˵����ݣ�����100

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
 * �������ƣ� JoyStickInit
 * ���������� ҡ��ֵ���㣬����ÿ���ᣬ��Buffer�е������ۼ�50��ȡƽ��ֵ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� struct JoystickerValue
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/3/14	     V1.0	  Ervin	      ����
 ***********************************************************************/

struct JoystickValue JoyStickValueCal(){
	uint8_t i;
	uint32_t x, y;
	struct JoystickValue value;
	for(i = 0,x =0,y=0; i < 100;)
	{
		x += ADC_Buffer[i++];
		y += ADC_Buffer[i++];
	}
	x /= 50;
	y /= 50;
	value.xValue = x;
	value.yValue = y;
	return value;
}	

void Joystick_Test(void){
	struct JoystickValue value;
	while(1){
		value = JoyStickValueCal();
		printf("xValue = %d, yValue = %d\n", value.xValue, value.yValue);
	}
}
