#include "can.h"
#include "driver_motorComm.h"

struct MotorStatus g_motorStatus;

/**********************************************************************
 * �������ƣ� MotorCommInit
 * ���������� ���ͨ�ų�ʼ��
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/3/14	     V1.0	  Ervin	      ����
 ***********************************************************************/



void MotorCommInit(){
	MyCAN_Init();
}

/**********************************************************************
 * �������ƣ� SendMessage2Motor
 * ���������� ��Motor����һ������֡
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/3/14	     V1.0	  Ervin	      ����
 ***********************************************************************/

void SendMessage2Motor(float voltage, uint8_t motorID)
{
	CAN_TxHeaderTypeDef header;
	header.IDE = CAN_ID_STD;
	header.RTR = CAN_RTR_DATA;
	header.DLC = 8;
	
	uint8_t data[8] = {0};
	
	if(motorID <= 4) //ID=1~4�����
	{
		header.StdId = 0x100;
		memcpy(&data[(motorID-1)*2], &(int16_t){voltage*1000}, 2);
	}
	else if(motorID > 4) //ID=5~8�����
	{
		header.StdId = 0x200;
		memcpy(&data[(motorID-5)*2], &(int16_t){voltage*1000}, 2);
	}
	
	uint32_t mailbox;
	HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox);
}

/**********************************************************************
 * �������ƣ� HAL_CAN_RxFifo0MsgPendingCallback
 * ���������� CAN�յ����ݵ��жϻص�
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2024/3/14	     V1.0	  Ervin	      ����
 ***********************************************************************/

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef header;
	uint8_t rxData[8];
	
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, rxData) != HAL_OK)
		return;

	
	g_motorStatus.angle = *(int32_t*)&rxData[0] / 1000.0f;
	g_motorStatus.speed = *(int16_t*)&rxData[4] / 10.0f;

}


void MotorComm_Test(void){
	while(1){
		SendMessage2Motor(2, 6);
		printf("angle is %f\n", g_motorStatus.angle);
		printf("speed is %f\n", g_motorStatus.speed);
	}
}
