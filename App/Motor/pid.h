#ifndef PID_H
#define PID_H

#include "stm32f4xx_hal.h"

typedef struct
{
   	float kp,ki,kd;//����ϵ��
    float error,lastError;//���ϴ����
    float integral,maxIntegral;//���֡������޷�
    float output,maxOutput;//���������޷�	
}PID;

/******************************************************************************/
float speed_loop(float target_speed, float current_speed);
float cascade_loop(float target_angle, float current_angle, float current_speed);
void MotorPIDInit(void);
void setIntens(uint8_t value);
/******************************************************************************/

#endif

