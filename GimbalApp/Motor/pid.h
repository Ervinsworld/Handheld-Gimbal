#ifndef PID_H
#define PID_H


typedef struct
{
   	float kp,ki,kd;//����ϵ��
    float error,lastError;//���ϴ����
    float integral,maxIntegral;//���֡������޷�
    float output,maxOutput;//���������޷�	
}PID;

/******************************************************************************/
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);
void PID_Calc(PID *pid,float reference,float feedback);
float speed_loop(float target_speed, float current_speed);
float cascade_loop(float target_angle, float current_angle, float current_speed);
void MotorPIDInit(void);
/******************************************************************************/

#endif

