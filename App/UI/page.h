#ifndef __PAGE_H
#define __PAGE_H

#include "stm32f4xx_hal.h"


#define Xres 128  //oled x�ֱ���
#define Yres 64	  //oled y�ֱ���
#define MainBMPres 48   //��ͼ������ֵ
#define ArrowBMPres 16  //��ͷͼ������ֵ��ֻ�л���ģʽӵ�м�ͷͼ��

#define PageNum 13
#define FatherPageNum 5 //��ҳ�������
#define MaxSonPageNum 4	//������ҳ�������

#define BarXStart 14
#define BarYStart 3
#define BarLength 100
//#define BarWidth  1

//����oled�����ܺ�ʱ����oled�����ܼ�ʱ����Ҫʹ�ö�����ʱ��������Է�ֹ��֡;�ú��vTaskDelay���ã���λΪtick
#define OLED_DELAY 50	

/*
ҳ������������֣�
	�������ͣ��˵�����Ӧҡ�������ƶ���ѭ��ˢ�µ�ҳ��
			 ��������ҳ�������֣�
				- pStatic �������л��������������
				- pSilde  �������޻�����ֻ�о�̬�ַ���������������
	�������ͣ�����������ҳ����¼������°�ť/��ת����ȣ���������ҳ��
			 ��������ҳ�������֣�
				- nStatic  ֻ��ʾ���棬���汾���������޽���
				- nBar 	   �н����ģ�����ʹ�ý���������
				- nDynamic �н����ģ�Ч����Ҫ����ʵ�֣�����Ϸ
*/


/************					ҳ�����ö����				************/
typedef enum
{
    pStatic = 0,
	pSlide= 1,
	nStatic = 2,
	nBar = 3,	  //��ֵ��ҳ��
	nDynamic = 4, //��̬ҳ�棬����Ϸҳ��
}InterfaceMode;

//ҳ����ţ���N��ҳ��,��c�ļ��в���ҳ��ľ�����Ϣ
typedef enum
{
    Light1 = 1,
	Off,
	On,
	Light2,
	Bright1,
	Bright2,
	Bright3,
	Light3,
	Game,
	Settings,
	LightBar,  //Light3������ҳ��
	BriBar, //��Ļ������ҳ��
	IntensBar //������ҳ��
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
int8_t sonPageSwitch(PageID targetPage);//��ҳ��Ļ�ҳ

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
void showbarFrame(void);//��ʾ��ֵ�����
int8_t showbar(void);//����pagedata��ʾ��ֵ��
//int8_t showbardata(void);//��ʾ�ײ�����
int8_t showBottomData(int8_t data);
void clearBar(void);
int8_t barInit(void);

//�ӷ���
void clearMainIcon(void);
void clearBottom(void);
void showString(void);
void clearString(void);
void showArrows(void);
void clearArrows(void);
void showBackArrow(void);
void clearBackArrow(void);

/*��Ļ����*/
void setScreenBri(uint8_t value);

//���ܺ���
char* Int2String(int num,char *str);
#endif