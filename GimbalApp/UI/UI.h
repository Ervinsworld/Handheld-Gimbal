#ifndef __UI_H
#define __UI_H

#include "stm32f4xx_hal.h"

#define Xres 128  //oled x�ֱ���
#define Yres 64	  //oled y�ֱ���
#define MainBMPres 48   //��ͼ������ֵ
#define ArrowBMPres 16  //��ͷͼ������ֵ��ֻ�л���ģʽӵ�м�ͷͼ��


//ҳ���ģʽ�������֣�����ģʽ��ȷ��ģʽ�Լ���̬ģʽ
typedef enum
{
    SlideMode = 0,
	EnterMode = 1,
	StaticMode = 2,
}InterfaceMode;

//ҳ����ţ�������ҳ��
typedef enum
{
    SwitchPage_EYEON = 0,
	SwitchPage_EYEOFF,
	SensPage,
	FlipPage,
	CameraPage,
	PoweroffPage,
	LockPage,
	UnlockPage,
	InitPage,
}PageID;

//ҳ����Ϣ�ṹ��
typedef struct UI_page{
	InterfaceMode InfMode;
	uint8_t nowadata;//ҳ�洫�ݵ�ʵʱ��Ϣ��ֻ�л���ģʽҳ������Ϣ��ȷ�ϼ���̬ģʽҳ���ֵĬ��Ϊ0
	const unsigned char* mainBMP;
}UIPage;

//ҳ���������
void showPage(UIPage* page, PageID id);

#endif
