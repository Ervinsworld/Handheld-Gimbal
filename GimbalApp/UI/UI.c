#include "driver_oled.h"
#include "driver_joystick.h"
#include "page.h"
#include "UI.h"
#include "FreeRTOS.h"

/**********************************************************************
 * �������ƣ� UI_Task
 * ���������� FreeRTOS����������ҳ����л�/��Ӧ�������
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/

void UI_Task(void *params){
	joystickValue value;
	int8_t SlideRet;//���������ķ���ֵ
	int8_t PageRet;//��ҳ�����ķ���ֵ
	UI_Init();//��ʼ��UI
	while(1){
		value = JoyStickValueCal();
		SlideRet = -1;//Ĭ��Ϊδ�ɹ�״̬
		PageRet = -1;//Ĭ��Ϊδ�ɹ�״̬
		//���ȴ��������
		if(value.xValue>3500){
			SlideRet = SlideRight();
		}
		else if(value.xValue<500){
			SlideRet = SlideRight();
		}
		//���ɹ�������
		if(SlideRet == 0){
			vTaskDelay(500);
		}
		//����ҳ����//
		if(value.yValue>3500){
			PageRet = PageDown();
		}
		else if(value.yValue<500){
			PageRet = PageUp();
		}
		//���ɹ�������
		if(PageRet == 0){
			vTaskDelay(500);
		}
		vTaskDelay(20);
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
	setCurrentpage(SwitchPage_EYEON);//��switchpage��Ϊ������ʼҳ��
	showPage();
}

/**********************************************************************
 * �������ƣ� PageDown
 * ���������� ����һҳ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0��ʾ�ɹ���-1��ʾʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/7	     V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t PageDown(){
	extern UIPage UIpages[10];
	uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	//����ҳ�Ǳ���״̬�����޷�ִ�з�ҳ����
	if(page.InfMode==nStatic)
		return -1;
	do{
		if(id==PageNum-1){
			id = 0;
		}
		else{
			id++;
		}
	}while(UIpages[id].InfMode==nStatic);
	setCurrentpage(id);
	clearPage();
	showPage();
	return 0;
}

/**********************************************************************
 * �������ƣ� PageUp
 * ���������� ����һҳ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0��ʾ�ɹ���-1��ʾʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/7      V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t PageUp(){
	extern UIPage UIpages[10];
	uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	//����ҳ�Ǳ���״̬�����޷�ִ�з�ҳ����
	if(page.InfMode==nStatic)
		return -1;
	do{
		if(id==0){
			id = PageNum-1;
		}
		else{
			id--;
		}

	}while(UIpages[id].InfMode==nStatic);
	setCurrentpage(id);
	clearPage();
	showPage();
	return 0;
}

/**********************************************************************
 * �������ƣ� SlideRight
 * ���������� ���һ���
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0��ʾ�ɹ���-1��ʾʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/7	     V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t SlideRight(){
	//uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	//����ҳ���ǻ���ҳ�棬���޷�ִ�л���
	if(page.InfMode!=pSilde)
		return -1;
	uint8_t num = page.datanum;
	uint8_t data = page.nowadata;
	if(data == num-1){
		data = 0;
	}
	else{
		data++;
	}
	setPagedata(data);
	clearString();
	showString();
	return 0;
}

/**********************************************************************
 * �������ƣ� SlideLeft
 * ���������� ���һ���
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0��ʾ�ɹ���-1��ʾʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/7	     V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t SlideLeft(){
	//uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	//����ҳ���ǻ���ҳ�棬���޷�ִ�л���
	if(page.InfMode!=pSilde)
		return -1;
	uint8_t num = page.datanum;
	uint8_t data = page.nowadata;
	if(data == 0){
		data = num-1;
	}
	else{
		data--;
	}
	setPagedata(data);
	clearString();
	showString();
	return 0;
}

