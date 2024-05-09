#include "pid.h"
#include "main.h"

static PID vel_pid, angle_pid;		//速度环和位置环pid定义

static void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);
static void PID_Calc(PID *pid,float reference,float feedback);

void MotorPIDInit(){
	//速度环和位置环pid初始化
	PID_Init(&vel_pid, 0.008, 0.0001, 0, 10000, 10000);//速度环采用pi控制
	//PID_Init(&angle_pid, 20, 0, 0.5,10000, 10000);//位置环采用pd控制,该组为稳定参数
	//PID_Init(&angle_pid, 37, 0, 0.5,10000, 10000);//相对激进，但有震荡的风险
	PID_Init(&angle_pid, 40, 0, 0.5,10000, 10000);
}

//单速度环控制,返回输出电压
float speed_loop(float target_speed, float current_speed){
	PID_Calc(&vel_pid, target_speed, current_speed);
	return vel_pid.output;
}

//串级控制环,返回输出电压
float cascade_loop(float target_angle, float current_angle, float current_speed){
	PID_Calc(&angle_pid, target_angle, current_angle);
	PID_Calc(&vel_pid, angle_pid.output, current_speed);
	return vel_pid.output;
}


//本质上就是设置角度环PID的P值,限制在20-60
void setIntens(uint8_t value){
	if(value < 20)
		value = 20;
	else if(value >60)
		value = 20;
	angle_pid.kp = value;
}

//用于初始化pid参数的函数
static void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;
}
 
//位置式 PID 算法
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
static void PID_Calc(PID *pid,float reference,float feedback)
{
 	//更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
    //计算微分
    float dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例
    float pout=pid->error*pid->kp;
    //计算积分
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}