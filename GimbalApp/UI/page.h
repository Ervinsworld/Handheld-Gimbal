#ifndef __PAGE_H
#define __PAGE_H

#include "stm32f4xx_hal.h"

#define Xres 128  //oled x�ֱ���
#define Yres 64	  //oled y�ֱ���
#define MainBMPres 48   //��ͼ������ֵ
#define ArrowBMPres 16  //��ͷͼ������ֵ��ֻ�л���ģʽӵ�м�ͷͼ��
#define PageNum 9
#define PosPageNum 5

/*
ҳ��������������������֣�
	�������ͣ��˵�����Ӧҡ�������ƶ���ѭ��ˢ�µ�ҳ��
			 ��������ҳ�������֣�
				- pStatic ���л��������������
				- pSilde  ���޻�����ֻ�о�̬�ַ���������������
	�������ͣ�����������ҳ����¼������°�ť����������ҳ��
			 ��������ҳ��ֻ��һ�֣�
				- nStatic

*/

typedef enum
{
    pStatic = 0,
	pSilde = 1,
	nStatic = 2,
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
	const uint8_t datanum; //ҳ�����Я������������������ҳ��Ϊҳ��Ļ�����������̬ҳ���ֵΪ0
	uint8_t nowadata;//ҳ�洫�ݵ�ʵʱ��Ϣ��ֻ�л���ģʽҳ������Ϣ��ȷ�ϼ���̬ģʽҳ���ֵĬ��Ϊ0
	const unsigned char* mainBMP;
}UIPage;

//ҳ���������
void PagesInfInit(void);
void setCurrentpage(PageID id);
UIPage getCurrentpage(void);
PageID getCurrentpageId(void);
int8_t setPagedata(uint8_t data);
void showPage(void);
void clearPage(void);
void clearMainIcon(void);
void clearBottom(void);
void showString(void);
void clearString(void);
void showArrows(void);
void clearArrows(void);
#endif
