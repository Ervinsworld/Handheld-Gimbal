#include "page.h"
#include "stm32f4xx_hal.h"
#include "driver_oled.h"
#include <string.h>
#include "FreeRTOS.h"
#include "Icons.c"


/************					ҳ����ر�				************/
//����ҳ���ϵ����ά����ÿ�еĵ�һ��Ԫ���Ǹ�ҳ�棬��������ҳ�棬�����ź�UI_page�ṹ���data��Ӧ
const uint8_t PageMapTable[FatherPageNum][MaxSonPageNum] = 
{
	{Light, Off, On},
	{Mode, Bright1, Bright2, Bright3},
	{Window, Bar},
};


//ҳ��ײ����ַ�����Ϣ������pSlideģʽ��ҳ�棬�������ź�UI_page�ṹ���data��Ӧ
const char* bottomStr[PageNum][3] = 
{
	{"Light", "", ""},
	{"OFF", "", ""},
	{"ON", "", ""},
	{"Mode", "", ""},
	{"Low", "", ""},
	{"Medium", "", ""},
	{"High", "", ""},
	{"Brightness", "", ""},
	{"Free", "Ratchet", "Fixed"},
	{"Block", "Car"},
};

UIPage UIpages[PageNum] = 
{	
{pStatic, 0, 1, BMP_LIGHT_48x48},//dataֵ1��ʾ�ƹ�رգ�2��ʾ�ƹ⿪��
{nStatic, 0, 0, BMP_OFF_48x48},
{nStatic, 0, 0, BMP_ON_48x48},
{pStatic, 0, 1, BMP_Sunny_48x48},//dataֵ1��2��3��������1��2��3
{nStatic, 0, 0, BMP_BRIGHT1_48x48},
{nStatic, 0, 0, BMP_BRIGHT2_48x48},
{nStatic, 0, 0, BMP_BRIGHT3_48x48},
{pStatic, 0, 0, BMP_WINDOW_48x48},//data�洢bar��ֵ��0-100������1
{pSlide, 3, 0, BMP_MOTOR_48x48},
{pSlide, 2, 0, BMP_SWITCH_48x48},
{nDynamic, 0, 0, NULL},
};


//��ǰҳ��Ľṹ������͵�ǰҳ���id
UIPage g_currentPage;
PageID g_currentId;

//��ͼ��λ�ò���
static uint8_t g_mainIcon_Xstart;
static uint8_t g_mainIcon_XEnd;
static uint8_t g_mainIcon_Ystart;
static uint8_t g_mainIcon_YEnd;

//�ײ���Ϣ�ַ�����Ϣ����
static uint8_t g_stringSize = 2;//�ַ���С�㶨Ϊ2
static uint8_t g_string_Ypos = 6;//�ַ�����ʾy����㶨Ϊ6
static uint8_t g_string_Xpos;
static uint8_t g_string_LenRes;

/**********************************************************************
 * �������ƣ� PageInfInit
 * ���������� ���㲢��ʼ��Page��ʾ�����ȫ�ֱ���
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void PagesInfInit(){
	//������ͼ��λ�ã���֤��ͼ���ڻ�������
	g_mainIcon_Xstart = ((Xres-MainBMPres)>>1) - 1;
	g_mainIcon_XEnd = g_mainIcon_Xstart + MainBMPres;
	g_mainIcon_Ystart = 0;
	g_mainIcon_YEnd = (g_mainIcon_Ystart + MainBMPres)>>3;
	
	//�ײ���Ϣ�ַ���λ�ò���,��Ϊ��ֵ
	g_string_Ypos = 6;//�ַ�����ʾy����㶨Ϊ6
	g_stringSize = 2;//�ַ���С�㶨Ϊ2
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
	uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	//����ҳ�Ǳ���״̬�����޷�ִ�з�ҳ����
	if((page.InfMode==nStatic) || (page.InfMode==nDynamic))
		return -1;
	do{
		if(id==PageNum){
			id = 1;
		}
		else{
			id++;
		}
	}while((UIpages[id-1].InfMode==nStatic) || (UIpages[id-1].InfMode==nDynamic));
	clearPage();
	setCurrentpage(id);
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
	uint8_t id = getCurrentpageId();
	UIPage page = getCurrentpage();
	//����ҳ�Ǳ���״̬�����޷�ִ�з�ҳ����
	if((page.InfMode==nStatic) || (page.InfMode==nDynamic))
		return -1;
	do{
		if(id==1){
			id = PageNum;
		}
		else{
			id--;
		}

	}while((UIpages[id-1].InfMode==nStatic) || (UIpages[id-1].InfMode==nDynamic));
	clearPage();
	setCurrentpage(id);
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
	if(page.InfMode!=pSlide)
		return -1;
	uint8_t num = page.slideNum;
	uint8_t data = page.data;
	if(data == num-1){
		data = 0;
	}
	else{
		data++;
	}
	setCurrentPagedata(data);
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
	if(page.InfMode!=pSlide)
		return -1;
	uint8_t num = page.slideNum;
	uint8_t data = page.data;
	if(data == 0){
		data = num-1;
	}
	else{
		data--;
	}
	setCurrentPagedata(data);
	clearString();
	showString();
	return 0;
}

/**********************************************************************
 * �������ƣ� PageIn
 * ���������� ������ҳ��
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� -1Ϊʧ��,���򷵻�0
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/

int8_t PageIn(){
	int i;
	clearPage();
	if(g_currentId == Window){
		vTaskDelay(OLED_DELAY);
		setCurrentpage(Bar);
		showbarFrame();
		showbardata();
		return 0;
	}
	for(i=0; i<FatherPageNum; i++){
		//��ѯҳ��ӳ���ϵ��
		if(PageMapTable[i][0] == g_currentId){
			setCurrentpage(PageMapTable[i][g_currentPage.data]);
			showPage();
			return 0;
		}
	}
	return -1;
}

/**********************************************************************
 * �������ƣ� PageOut
 * ���������� ���ظ�ҳ��
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� -1Ϊʧ�ܣ�0Ϊ�ɹ�
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/

int8_t PageOut(){
	int i, j;
	//��ΪBarҳ��
	if(g_currentId == Bar){
		vTaskDelay(OLED_DELAY);
		clearBar();
		vTaskDelay(OLED_DELAY);
		clearString();
		vTaskDelay(OLED_DELAY);
		setCurrentpage(Window);
		showPage();
		return 0;
	}
	//��Ϊ����ҳ��
	for(i=0; i<FatherPageNum; i++){
		for(int j=1; j<MaxSonPageNum; j++){
			if(PageMapTable[i][j] == getCurrentpageId()){
				clearPage();
				setCurrentpage(PageMapTable[i][0]);
				showPage();
				return 0;
			}
		}
	}
	return -1;
}


/**********************************************************************
 * �������ƣ� setCurrentpage
 * ���������� ���õ�ǰҳ�棬д��ȫ�ֱ���
 * ��������� pageID
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void setCurrentpage(PageID id){
	//g_currentPage = UIpages[id];
	memcpy(&g_currentPage, &UIpages[id-1], sizeof(g_currentPage));
	g_currentId = id;
}

/**********************************************************************
 * �������ƣ� getCurrentpage
 * ���������� ��ȡ��ǰҳ��ṹ��
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� UIPage
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
 UIPage getCurrentpage(){
	return g_currentPage;
}

/**********************************************************************
 * �������ƣ� getCurrentpageId
 * ���������� ��ȡ��ǰҳ��ṹ��Id
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� PageID
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
PageID getCurrentpageId(){
	return g_currentId;
}

/**********************************************************************
 * �������ƣ� setCurrentPagedata
 * ���������� ���õ�ǰҳ�������ֵ
 * ��������� data
 * ��������� ��
 * �� �� ֵ�� -1��ʾʧ�ܣ�0��ʾ�ɹ�
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/7	     V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t setCurrentPagedata(uint8_t data){
	//�ж��趨ֵ�Ƿ񳬹�������ֵ�����������������������-1
	if(g_currentPage.InfMode == pSlide && data >= g_currentPage.slideNum)
		return -1;
	else{
		g_currentPage.data = data;
		UIpages[g_currentId-1].data = data;
	}
	return 0;
}

/**********************************************************************
 * �������ƣ� setPagedata
 * ���������� ����ָ��ҳ�������ֵ
 * ��������� id, data
 * ��������� ��
 * �� �� ֵ�� -1��ʾʧ�ܣ�0��ʾ�ɹ�
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/7	     V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t setPagedata(uint8_t id, uint8_t data){
	//�ж��趨ֵ�Ƿ񳬹�������ֵ�����������������������-1
	UIPage page = UIpages[id-1];
	if(page.InfMode == pSlide && data >= page.slideNum)
		return -1;
	else 
		UIpages[id-1].data = data;
	return 0;
}

/**********************************************************************
 * �������ƣ� showPage
 * ���������� ҳ����ʾ
 * ��������� UIPage�ṹ�壬������
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void showPage(){
	//��Ϊ����ģʽ����ʾ��ͷ
	if(g_currentPage.InfMode == pSlide)
		showArrows();
	else if(g_currentPage.InfMode == nStatic||g_currentPage.InfMode == nDynamic)
		showBackArrow();
	showString();
	OLED_DrawBMP(g_mainIcon_Xstart,g_mainIcon_Ystart,g_mainIcon_XEnd,g_mainIcon_YEnd,g_currentPage.mainBMP);
	//OLED_DrawBMP(39,0,87,6,BMP_Camera_48x48);
}

/**********************************************************************
 * �������ƣ� clearPage
 * ���������� �����ҳ���ݣ�ȫ�����̫��ʱ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearPage(){
	clearMainIcon();
	clearString();
	if(g_currentPage.InfMode == pSlide)
		clearArrows();
	else if(g_currentPage.InfMode == nStatic||g_currentPage.InfMode == nDynamic)
		clearBackArrow();
}

/**********************************************************************
 * �������ƣ� showbar
 * ���������� ��ֵ����ʾ����
 * ��������� ��ֵ����ֵ����0-100
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/19	     V1.0	  Ervin	      ����
 ***********************************************************************/

int8_t showbar(){
	if(g_currentId != Bar)
		return -1;
	static uint8_t last_value = 0;
	int8_t error;
	uint8_t i, j;
	error = g_currentPage.data - last_value;
	if(error>0){
		for(i=0; i<BarWidth; i++){
			OLED_SetPos(BarXStart+last_value,BarYStart+i);
			for(j=0;j<error;j++)
				WriteDat(0xFF);
		}
	}
	else if(error<0){
		for(i=0; i<BarWidth; i++){
			OLED_SetPos(BarXStart+g_currentPage.data,BarYStart+i);
			for(j=0;j<-error;j++)
				WriteDat(0x81);
		}
	}
	last_value = g_currentPage.data;
	return 0;
}

/**********************************************************************
 * �������ƣ� showbarFrame
 * ���������� ��ֵ���߿���ʾ����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/19	     V1.0	  Ervin	      ����
 ***********************************************************************/
void showbarFrame(){
	uint8_t i, j;
	for(i=0; i<BarWidth; i++){
		OLED_SetPos(BarXStart,BarYStart+i);
		for(j=0;j<BarLength;j++){
			if(j==0||j==BarLength-1)
				WriteDat(0xFF);
			else
				WriteDat(0x81);
		}
	}
}

/**********************************************************************
 * �������ƣ� clearBar
 * ���������� ��������Bar
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/23	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearBar(){
	uint8_t i, j;
	for(i=0; i<BarWidth; i++){
		OLED_SetPos(BarXStart,BarYStart+i);
		for(j=0;j<BarLength;j++){
			WriteDat(0x00);
		}
	}
}

/**********************************************************************
 * �������ƣ� showbardata
 * ���������� ��ֵ���ײ���ֵ����ʾ����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0Ϊ�ɹ���-1Ϊʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/19	     V1.0	  Ervin	      ����
 ***********************************************************************/
int8_t showbardata(){
	char s[7] = {0};
	char result_s[7] = " ";
	if(g_currentId!=Bar)
		return -1;
	Int2String(g_currentPage.data, s);
	//��ת����ɵ�����ǰ�󶼼���һ���ո� ��
	strcat(s, " ");
	strcat(result_s, s);
	//�����ַ�����ʾ��������ʾ�ַ�
	uint8_t len = strlen(result_s);
	g_string_LenRes = len<<3;
	g_string_Xpos = ((Xres-g_string_LenRes)>>1) - 1;
	OLED_ShowStr(g_string_Xpos, g_string_Ypos, (unsigned char*)result_s, g_stringSize);
}

/**********************************************************************
 * �������ƣ� Int2String
 * ���������� ����ת�ַ�����
 * ��������� ����
 * ��������� �ַ����׵�ַ
 * �� �� ֵ�� �ַ����׵�ַ
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/19	     V1.0	  Ervin	      ����
 ***********************************************************************/
char* Int2String(int num,char *str)//10���� 
{
    int i = 0;//ָʾ���str 
    if(num<0)//���numΪ��������num���� 
    {
        num = -num;
        str[i++] = '-';
    } 
    //ת�� 
    do
    {
        str[i++] = num%10+48;//ȡnum���λ �ַ�0~9��ASCII����48~57������˵����0+48=48��ASCII���Ӧ�ַ�'0' 
        num /= 10;//ȥ�����λ    
    }while(num);//num��Ϊ0����ѭ��
    
    str[i] = '\0';
    
    //ȷ����ʼ������λ�� 
    int j = 0;
    if(str[0]=='-')//����и��ţ����Ų��õ��� 
    {
        j = 1;//�ӵڶ�λ��ʼ���� 
        ++i;//�����и��ţ����Խ����ĶԳ���ҲҪ����1λ 
    }
    //�Գƽ��� 
    for(;j<i/2;j++)
    {
        //�Գƽ������˵�ֵ ��ʵ����ʡ���м��������a+b��ֵ��a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;//����ת�����ֵ 
}

/**********************************************************************
 * �������ƣ� clearMainIcon
 * ���������� �����ͼ��48*48���ص�����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearMainIcon(){
	uint8_t i, j;
	for(i=0; i<6; i++){
		OLED_SetPos(g_mainIcon_Xstart,g_mainIcon_Ystart+i);
		for(j=0;j<MainBMPres;j++)
			WriteDat(0x00);
	}
}

/**********************************************************************
 * �������ƣ� clearString
 * ���������� ����ײ��ַ�������Ĭ��sizeΪ2
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearString(){
	uint8_t i, j;
	for(i=0; i<2; i++){
		OLED_SetPos(g_string_Xpos,g_string_Ypos+i);
		for(j=0;j<g_string_LenRes;j++)
			WriteDat(0x00);
	}
}

/**********************************************************************
 * �������ƣ� showString
 * ���������� �����ַ���Ϣ����ȫ�ֱ���������ʾ�ײ��ַ���
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void showString(){
	if(g_currentPage.InfMode == pSlide){
		uint8_t len = strlen(bottomStr[g_currentId-1][g_currentPage.data]);
		g_string_LenRes = len<<3;
		g_string_Xpos = ((Xres-g_string_LenRes)>>1) - 1;
		OLED_ShowStr(g_string_Xpos, g_string_Ypos, (unsigned char*)bottomStr[g_currentId-1][g_currentPage.data], g_stringSize);
	}
	else{
		uint8_t len = strlen(bottomStr[g_currentId-1][0]);
		g_string_LenRes = len<<3;
		g_string_Xpos = ((Xres-g_string_LenRes)>>1) - 1;
		OLED_ShowStr(g_string_Xpos, g_string_Ypos, (unsigned char*)bottomStr[g_currentId-1][0], g_stringSize);
	}
	
}



/**********************************************************************
 * �������ƣ� clearBottom
 * ���������� ����ײ���������
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearBottom(){
	clearArrows();
	clearString();
}
/**********************************************************************
 * �������ƣ� clearArrows
 * ���������� ����ײ���ͷ����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearArrows(){
	uint8_t i, j;
	for(i=0; i<2; i++){
		OLED_SetPos(0,6+i);
		for(j=0;j<ArrowBMPres;j++)
			WriteDat(0x00);
	}
	for(i=0; i<2; i++){
		OLED_SetPos(Xres-ArrowBMPres,6+i);
		for(j=0;j<ArrowBMPres;j++)
			WriteDat(0x00);
	}
}

/**********************************************************************
 * �������ƣ� showArrows
 * ���������� ����ģʽ��ҳ����ʾ���Ҽ�ͷ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/6	     V1.0	  Ervin	      ����
 ***********************************************************************/
void showArrows(){
	OLED_DrawBMP(0,6,ArrowBMPres,8,BMP_Left_16x16);//��ʾ���ͷ
	OLED_DrawBMP(Xres-ArrowBMPres,6,Xres,8,BMP_Right_16x16);//��ʾ�Ҽ�ͷ
}

/**********************************************************************
 * �������ƣ� clearBackArrow
 * ���������� ������ؼ�ͷ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/17	     V1.0	  Ervin	      ����
 ***********************************************************************/
void clearBackArrow(){
	uint8_t i, j;
	for(i=0; i<2; i++){
		OLED_SetPos(0,i);
		for(j=0;j<ArrowBMPres;j++)
			WriteDat(0x00);
	}
}

/**********************************************************************
 * �������ƣ� showBackArrow
 * ���������� ��ҳ����ʾ���ؼ�ͷ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/4/17	     V1.0	  Ervin	      ����
 ***********************************************************************/
void showBackArrow(){
	OLED_DrawBMP(0,0,ArrowBMPres,2,BMP_Left_16x16);
}