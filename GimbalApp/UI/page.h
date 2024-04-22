#ifndef __PAGE_H
#define __PAGE_H

#include "stm32f4xx_hal.h"


#define Xres 128  //oled x�ֱ���
#define Yres 64	  //oled y�ֱ���
#define MainBMPres 48   //��ͼ������ֵ
#define ArrowBMPres 16  //��ͷͼ������ֵ��ֻ�л���ģʽӵ�м�ͷͼ��

#define PageNum 10
#define FatherPageNum 5
#define MaxSonPageNum 4

#define BarXStart 14
#define BarYStart 2
#define BarLength 100
#define BarWidth  1
/*
ҳ������������֣�
	�������ͣ��˵�����Ӧҡ�������ƶ���ѭ��ˢ�µ�ҳ��
			 ��������ҳ�������֣�
				- pStatic �������л��������������
				- pSilde  �������޻�����ֻ�о�̬�ַ���������������
	�������ͣ�����������ҳ����¼������°�ť/��ת����ȣ���������ҳ��
			 ��������ҳ��Ҳ�����֣�
				- nStatic  ֻ��ʾ�޽���
				- nDynamic �н�����
*/


/************					ҳ�����ö����				************/
typedef enum
{
    pStatic = 0,
	pSlide= 1,
	nStatic = 2,
	nDynamic = 3
}InterfaceMode;

//ҳ����ţ���ʮ��ҳ��,��c�ļ��в���ҳ��ľ�����Ϣ
typedef enum
{
    Light = 1,
	Off,
	On,
	Mode,
	Bright1,
	Bright2,
	Bright3,
	Window,
	MotorPlay,
	Game,
}PageID;

/************					ҳ����Ϣ�ṹ��				************/
typedef struct UI_page{
	InterfaceMode InfMode;
	const uint8_t slideNum; //ҳ��Ļ�������������ֻ��pSlideģʽҳ���У�����Ϊ0
	uint8_t data;//pSlideҳ�洫�ݻ�ҳ��Ϣ������ҳ�洫���Զ�����Ϣ
	const unsigned char* mainBMP;
}UIPage;


/************					ҳ���������					************/
//��Ϣ��ʼ������
void PagesInfInit(void);

//����ת�Ʒ���
int8_t PageDown(void);
int8_t PageUp(void);
int8_t SlideRight(void);
int8_t SlideLeft(void);
int8_t PageIn(void);
int8_t PageOut(void);

//ȫ�ֱ���get/set����
void setCurrentpage(PageID id);
UIPage getCurrentpage(void);
PageID getCurrentpageId(void);
int8_t setCurrentPagedata(uint8_t data);
int8_t setPagedata(uint8_t id, uint8_t data);

//λͼ��ʾ���������
void showPage(void);
void clearPage(void);

//��ֵ����������
void showbarFrame(void);
void showbar(void);
int8_t showbardata(void);

//����Ҫʹ�õ��ӷ���
void clearMainIcon(void);
void clearBottom(void);
void showString(void);
void clearString(void);
void showArrows(void);
void clearArrows(void);
void showBackArrow(void);
void clearBackArrow(void);

char* Int2String(int num,char *str);
#endif
