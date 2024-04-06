#include "driver_oled.h"
#include "UI.h"
#include "menu.h"
/**********************************************************************
 * �������ƣ� MenuInit
 * ���������� ��ʼ��UI�˵�������ʾ��ҳ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void MenuInit(){
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
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void PageDown(){
	extern UIPage UIpages[10];
	uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	do{
		id++;
		if(id==PageNum-1){
			id = 0;
		}
	}while(UIpages[id].InfMode==nStatic);
	setCurrentpage(id);
	clearPage();
	showPage();
}

/**********************************************************************
 * �������ƣ� PageUp
 * ���������� ����һҳ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void PageUp(){
	extern UIPage UIpages[10];
	uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	do{
		id--;
		if(id==0){
			id = PageNum-1;
		}
	}while(UIpages[id].InfMode==nStatic);
	setCurrentpage(id);
	clearPage();
	showPage();
}
