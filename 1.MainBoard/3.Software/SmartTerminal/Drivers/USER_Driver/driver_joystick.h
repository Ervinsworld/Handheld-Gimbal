#ifndef __DRIVER_JOYSTICKER_H
#define __DRIVER_JOYSTICKER_H

#include "stm32f4xx_hal.h"

//ҡ��X,Y��ֵ�ṹ�壬ÿ����ֵ��Χ��0~4096,��ֵ2000-2100
//X����С�Ҵ�Y����С�´�
typedef struct JoystickValue{
	uint32_t xValue;
	uint32_t yValue;
}joystickValue;

typedef enum {
	free = 0,
	pressed = 1,
}buttonState;

void JoyStickInit(void);
joystickValue JoyStickValueCal(void);
void Joystick_Test(void *params);
	
#endif
