#include "driver_oled.h"
#include "driver_joystick.h"
#include "page.h"
#include "UI.h"
#include "driver_motorComm.h"
#include "pid.h"
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

extern TaskHandle_t MotorPidTaskHandle;
extern QueueHandle_t TargetAngleQueueHandle;
extern QueueHandle_t AngleDiffQueueHandle;
extern EventGroupHandle_t UIResponseEvent;
extern SemaphoreHandle_t UIActionSemaphore;
extern SemaphoreHandle_t MotorPidSemaphore;

void UIAction_Task(void *params){ 
	PageID id;
	float targetAngle = 0;
	float angleDiff = 0;
	while(1){
		//�ȴ����°���������ҳ�棬���ź��������������������ͣ
		xSemaphoreTake(UIActionSemaphore, portMAX_DELAY);
		xQueuePeek(AngleDiffQueueHandle, &angleDiff, portMAX_DELAY);
		id = getCurrentpageId();
		//printf("%f,%f\n", motorInf.angle, targetAngle);
		
		//��ΪnStaticҳ��
		if(getCurrentpage().InfMode == nStatic){	
			switch(id){
				case On:{
					if(g_currentMotorInf.angle>-_3_PI_4 - angleDiff){
						targetAngle = 0 - angleDiff;
						xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
						sonPageSwitch(Off);
						vTaskDelay(pdMS_TO_TICKS(1000));
					}
					break;
				}

				case Off:{
					if(g_currentMotorInf.angle<-_PI_4 - angleDiff){
						targetAngle = -_PI - angleDiff;
						xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
						sonPageSwitch(On);
						vTaskDelay(pdMS_TO_TICKS(1000));
					}
					break;
				}
				
				case Bright1:{
					if(g_currentMotorInf.angle<_3_PI_4 - angleDiff){
						targetAngle = 0 - angleDiff;
						xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
						sonPageSwitch(Bright2);
						vTaskDelay(pdMS_TO_TICKS(1000));
					}
					break;
				}
				
				case Bright2:{
					if(g_currentMotorInf.angle>_PI_4 - angleDiff){
						targetAngle = _PI - angleDiff;
						xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
						sonPageSwitch(Bright1);
						vTaskDelay(pdMS_TO_TICKS(1000));
					}
					else if(g_currentMotorInf.angle<-_PI_4 - angleDiff){
						targetAngle = -_PI - angleDiff;
						xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
						sonPageSwitch(Bright3);
						vTaskDelay(pdMS_TO_TICKS(1000));
					}
					break;
				}
				
				case Bright3:{
					if(g_currentMotorInf.angle>-_3_PI_4 - angleDiff){
						targetAngle = 0 - angleDiff;
						xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
						sonPageSwitch(Bright2);
						vTaskDelay(pdMS_TO_TICKS(1000));
					}
					break;
				}
				default: break;	
			}
		}
		else if(getCurrentpage().InfMode == nBar){
			//PI�� -PIӳ�䵽0-100
			UIPage page = getCurrentpage();
			float value = page.data;//bar��ʾ��ֵ��0-100
			float minAngle = _PI - angleDiff; //��С�߽�ֵ
			float maxAngle = -_PI - angleDiff;  //���߽�ֵ
			//������Ƕ�ֵת��Ϊvalueֵ
			value = -(g_currentMotorInf.angle + angleDiff)*(50/_PI) + 50;
			
			if(value>=-5&&value<105){
				//ȷ��pid������ͣ������ϵ�
				xSemaphoreTake(MotorPidSemaphore, 0);
				SendMessage2Motor(0 ,motorID);
				//���͵�ǰ�Ƕ�ΪĿ��Ƕ��ѱ��˳�ʹ��
				targetAngle = g_currentMotorInf.angle;
				xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
				if(value<0)
					setCurrentPagedata(0);
				else if(value > 100)
					setCurrentPagedata(100);
				else
					setCurrentPagedata(value);
				switch(id){
					case LightBar: break;
					case BriBar: setScreenBri((float)getCurrentpage().data*2.55);break;
					case IntensBar:setIntens((float)getCurrentpage().data*0.4+20);break;
					default : break;
					
				}
				showbar();
				//clearString();
				showBottomData(getCurrentpage().data);
			}
			else if(value<-5){
				xQueueOverwrite(TargetAngleQueueHandle, &minAngle);
				xSemaphoreGive(MotorPidSemaphore);
			}
			else if(value>=105){
				xQueueOverwrite(TargetAngleQueueHandle, &maxAngle);
				xSemaphoreGive(MotorPidSemaphore);
			}
		}
		xSemaphoreGive(UIActionSemaphore);
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
					id = getCurrentpageId();
					//��Ϊ�ض�ҳ����ʹ�ܵ��pid
					//vTaskDelay(3000);
					PageIn();
					//vTaskDelay(3000);
					id = getCurrentpageId();
					//vTaskDelay(3000);
					//printf("current id1: %d", id);
					if(getCurrentpage().InfMode == nStatic){
						switch(id){
							case On: diffAngle = -_PI - TargetAngle;	break;
							case Off: diffAngle = 0 - TargetAngle;	break;
							case Bright1: diffAngle = _PI - TargetAngle;	break;
							case Bright2: diffAngle = 0 - TargetAngle;	break;
							case Bright3: diffAngle = -_PI - TargetAngle;	break;
							default:break;	
						}
					}
					else if(getCurrentpage().InfMode == nBar){
						//vTaskDelay(1000);
						diffAngle = -(float)getCurrentpage().data*_PI/50 + _PI - TargetAngle;
						//printf("bar diffangle is %f:\n",diffAngle);
					}
					//printf("current id2: %d", id);
					xQueueOverwrite(AngleDiffQueueHandle, &diffAngle);
					//vTaskDelay(pdMS_TO_TICKS(2000));
					xSemaphoreGive(UIActionSemaphore);
					//printf("%f, %f\n", targetAngle, g_currentMotorInf.angle);	
				}
				//�ɹ����ӳٲ��������λ
				//printf("bit:%d\n", UIResponseEventbit);
				vTaskDelay(pdMS_TO_TICKS(700));
				xEventGroupClearBits(UIResponseEvent, 1<<0|1<<1|1<<2|1<<3|1<<4);
			}
			
		}
		//��Ϊ��ҳ��,ҡ���ƶ����ⷽ����½Կ��˳�����ҳ�����
		else{
			xEventGroupWaitBits(UIResponseEvent, 1<<0|1<<1|1<<2|1<<3, pdTRUE, pdFALSE, portMAX_DELAY);
			//xQueueOverwrite(TargetAngleQueueHandle, &targetAngle);
			xSemaphoreTake(UIActionSemaphore, portMAX_DELAY);//ֹͣ����ִ������
			//Barҳ����ָ����ʹ��
			if(getCurrentpage().InfMode == nBar){
				xSemaphoreGive(MotorPidSemaphore);
			}
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
 * �������ƣ� UIPrint_Task
 * ���������� UI��Ϣ�Ĵ�ӡ���Ժ���
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/

void UIPrint_Task(void *params){
	while(1){
		printf("%d\n", getCurrentpageId());
		vTaskDelay(pdMS_TO_TICKS(50));
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
	setCurrentpage(Light1);//��switchpage��Ϊ������ʼҳ��
	showPage();
	vTaskDelay(1000);
}

