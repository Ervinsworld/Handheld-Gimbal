#ifndef __UI_H
#define __UI_H

#include "stm32f4xx_hal.h"

/*FreeRTOS������*/
void UI_Task(void *params);

/*��������*/
void UI_Init(void);
int8_t PageDown(void);
int8_t PageUp(void);
int8_t SlideRight(void);
int8_t SlideLeft(void);
int8_t PageAction(void);
#endif
