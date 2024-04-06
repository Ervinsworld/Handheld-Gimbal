#include "UI.h"
#include "Icons.c"
#include "stm32f4xx_hal.h"
#include "driver_oled.h"
#include <string.h>

PageID id;
InterfaceMode Mode;

//ҳ��ײ����ַ�����Ϣ������UI_page�ṹ���������������ֵ�����ַ�����Ϣ
const char* bottomStr[10][3] = 
{
	{"Switch", "", ""},
	{"Switch", "", ""},
	{"Low", "Meduim", "High"},
	{"Flip", "", ""},
	{"OFF", "ON", ""},
	{"Power off", "", ""},
	{"Hold to Unlock", "", ""},
	{"Unlocking...", "", ""},
	{"Initing...", "", ""},
};

UIPage UIpages[10] = 
{	
{EnterMode, 0, BMP_Eye_48x48},
{EnterMode, 0, BMP_EyeBlocked_48x48},
{SlideMode, 0, BMP_Meter_48x48},
{EnterMode, 0, BMP_Flip_48x48},
{SlideMode, 0, BMP_Camera_48x48},
{EnterMode, 0, BMP_Power_48x48},
{EnterMode, 0, BMP_Lock_48x48},
{StaticMode, 0, BMP_Unlocked_48x48},
{StaticMode, 0, BMP_Spinner_48x48},
};



//�����ַ������ȼ����ַ���X�����Ա�֤�ַ��ڻ�������
uint8_t String_XPos(const char * str){
	uint8_t len = strlen(str);
	return ((Xres-(len<<3))>>1) - 1;
}


/**********************************************************************
 * �������ƣ� showPage
 * ���������� ҳ����ʾ����
 * ��������� UIPage�ṹ�壬������
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void showPage(UIPage* page, PageID id){
	UIPage tmpPage = page[id];
	//������ͼ��λ�ã���֤��ͼ���ڻ�������
	uint8_t mainIcon_Xstart = ((Xres-MainBMPres)>>1) - 1;
	uint8_t mainIcon_XEnd = mainIcon_Xstart + MainBMPres;
	uint8_t mainIcon_Ystart = 0;
	uint8_t mainIcon_YEnd = (mainIcon_Ystart + MainBMPres)>>3;
	
	//�ײ���Ϣ�ַ���λ�ò���
	uint8_t String_Ypos = 6;//�ַ�����ʾy����㶨Ϊ6
	uint8_t StringSize = 2;//�ַ���С�㶨Ϊ2
	uint8_t String_Xpos = String_XPos(bottomStr[id][tmpPage.nowadata]);

	//OLED_ShowStr(40, 6, pstr, 2);
	OLED_ShowStr(String_Xpos, String_Ypos, (unsigned char*)bottomStr[id][tmpPage.nowadata], StringSize);
	OLED_DrawBMP(mainIcon_Xstart,mainIcon_Ystart,mainIcon_XEnd,mainIcon_YEnd,tmpPage.mainBMP);
}

