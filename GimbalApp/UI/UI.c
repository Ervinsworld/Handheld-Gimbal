#include "driver_oled.h"
#include "driver_joystick.h"
#include "page.h"
#include "UI.h"
#include "driver_motorComm.h"
#include "Motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core

/**********************************************************************
 * �������ƣ� UIAction_Task
 * ���������� ����ҳ����ִ����Ӧ����Ϊ����Ҫ�ǵ������Ļ�Ľ���
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/19	     V1.0	  Ervin	      ����
 ***********************************************************************/

extern QueueHandle_t TargetAngleQueueHandle;
extern QueueHandle_t AngleDiffQueueHandle;
extern EventGroupHandle_t UIResponseEvent;
extern EventGroupHandle_t UIActionEvent;
extern TaskHandle_t MotorPidTaskHandle;

void UIAction_Task(void *params){ 
	PageID id;
	float targetAngle = 0;
	float angleDiff = 0;
	while(1){
		//�ȴ����°���������ҳ�棬���¼������������������ͣ
		xEventGroupWaitBits(UIActionEvent, 1<<0, pdFALSE, pdFALSE, portMAX_DELAY);
		xQueuePeek(AngleDiffQueueHandle, &angleDiff, portMAX_DELAY);
		id = getCurrentpageId();
		//printf("%f,%f\n", motorInf.angle, targetAngle);
		switch(id){
			case On:{
				if(g_currentMotorInf.angle>-_3_PI_4 - angleDiff){
					targetAngle = 0 - angleDiff;
					xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
					clearPage();
					setCurrentpage(Off);
					setPagedata(Light, 1);
					showPage();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				break;
			}

			case Off:{
				if(g_currentMotorInf.angle<-_PI_4 - angleDiff){
					targetAngle = -_PI - angleDiff;
					xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
					clearPage();
					setCurrentpage(On);
					setPagedata(Light, 2);
					showPage();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				break;
			}
			
			case Bright1:{
				if(g_currentMotorInf.angle<_3_PI_4 - angleDiff){
					targetAngle = 0 - angleDiff;
					xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
					clearPage();
					setCurrentpage(Bright2);
					setPagedata(Mode, 2);
					showPage();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				break;
			}
			
			case Bright2:{
				if(g_currentMotorInf.angle>_PI_4 - angleDiff){
					targetAngle = _PI - angleDiff;
					xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
					clearPage();
					setCurrentpage(Bright1);
					setPagedata(Mode, 1);
					showPage();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				else if(g_currentMotorInf.angle<-_PI_4 - angleDiff){
					targetAngle = -_PI - angleDiff;
					xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
					clearPage();
					setCurrentpage(Bright3);
					setPagedata(Mode, 3);
					showPage();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				break;
			}
			
			case Bright3:{
				if(g_currentMotorInf.angle>-_3_PI_4 - angleDiff){
					targetAngle = 0 - angleDiff;
					xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
					clearPage();
					setCurrentpage(Bright2);
					setPagedata(Mode, 2);
					showPage();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				break;
			}
			
			//PI�� -PIӳ�䵽0-100
			/*
			case Window:{
				static int last_value = 0; 
				int value = ((_PI-g_currentMotorInf.angle)/_PI)*50;

				//printf("%d\n", value);
				printf("%d\n", getCurrentpage().data);
				if(value>=0&&value<=100&&(last_value!=value)){
					vTaskSuspend(MotorPidTaskHandle);
					SendMessage2Motor(0, motorID);
					setPagedata(value);
					showbar();
			    	clearString();
					showbardata();
				}
				last_value = value;
				break;
			}
			*/
			
			default:
				break;
		}
		//printf("%f\n", targetAngle);
	}
}


/**********************************************************************
 * �������ƣ� UI_Task
 * ���������� FreeRTOS����������ҳ����л��������
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/

void UI_Task(void *params){
	EventBits_t UIResponseEventbit;
	PageID id;
	//float targetAngle = 0;
	float TargetAngle = 0;
	float diffAngle = 0;
	UI_Init();//��ʼ��UI
	while(1){
		InterfaceMode mode = getCurrentpage().InfMode;
		//��Ϊ��ҳ��
		if((mode == pStatic)||(mode == pSlide)){
			UIResponseEventbit = xEventGroupWaitBits(UIResponseEvent, 1<<0|1<<1|1<<2|1<<3|1<<4, pdTRUE, pdFALSE, portMAX_DELAY);
			if(UIResponseEventbit){
				//���ȷ�ҳ
				if(UIResponseEventbit&(1<<2))
					PageDown();
				else if(UIResponseEventbit&(1<<3))
					PageUp();
				else if(UIResponseEventbit&(1<<1))
					SlideLeft();
				else if(UIResponseEventbit&(1<<0))
					SlideRight();
				else if(UIResponseEventbit&(1<<4)){
					xQueuePeek(TargetAngleQueueHandle, &TargetAngle, 0);				
					//���ø���ҳ�����ʱ��Ŀ��Ƕ�
					PageIn();
					id = getCurrentpageId();
					switch(id){
						/*
						case On: targetAngle = -_PI_2;break;
						case Off: targetAngle = 0;break;
						case Bright1: targetAngle = _PI_2;break;
						case Bright2: targetAngle = 0;break;
						case Bright3: targetAngle = -_PI_2;break;
						*/
						case On: diffAngle = -_PI - TargetAngle;	break;
						case Off: diffAngle = 0 - TargetAngle;	break;
						case Bright1: diffAngle = _PI - TargetAngle;	break;
						case Bright2: diffAngle = 0 - TargetAngle;	break;
						case Bright3: diffAngle = -_PI - TargetAngle;	break;
						default:break;
					}
					xQueueOverwrite(AngleDiffQueueHandle, &diffAngle);
					//vTaskDelay(pdMS_TO_TICKS(2000));
					xEventGroupSetBits(UIActionEvent, 1<<0);
					//printf("%f, %f\n", targetAngle, g_currentMotorInf.angle);	
				}
				//�ɹ����ӳٲ��������λ
				//printf("bit:%d\n", UIResponseEventbit);
				vTaskDelay(pdMS_TO_TICKS(1000));
				xEventGroupClearBits(UIResponseEvent, 1<<0|1<<1|1<<2|1<<3|1<<4);
			}
			
		}
		//��Ϊ��ҳ��,ҡ���ƶ����ⷽ����½Կ��˳�����ҳ�����
		else{
			xEventGroupWaitBits(UIResponseEvent, 1<<0|1<<1|1<<2|1<<3|1<<4, pdTRUE, pdFALSE, portMAX_DELAY);
			//xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
			xEventGroupClearBits(UIActionEvent, 1<<0);//ֹͣ����ִ������
			PageOut();
			//printf("I am out\n");
			//�ɹ����ӳ�0.5�벢�������λ
			vTaskDelay(pdMS_TO_TICKS(500));
			xEventGroupClearBits(UIResponseEvent, 1<<0|1<<1|1<<2|1<<3|1<<4);//ҡ����0
			
		}
		//printf("%d\n", bit);
	}
}


/**********************************************************************
 * �������ƣ� UI_Init
 * ���������� ��ʼ��UI�˵�������ʾ��ҳ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void UI_Init(){
	OLED_Init();
	OLED_CLS();
	PagesInfInit();
	setCurrentpage(Light);//��switchpage��Ϊ������ʼҳ��
	showPage();
}

