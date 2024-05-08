#include "driver_timer.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"

extern void JoystickButton_IRQ_Callback(void);

/**********************************************************************
 * �������ƣ� HAL_GPIO_EXTI_Callback
 * ���������� �ⲿ�жϵ��жϻص�����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸����ڣ�      �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2023/08/04	     V1.0	  Τ��ɽ	      ����
 ***********************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case Button_Pin:
        {
            JoystickButton_IRQ_Callback();
            break;
        }
        default:
        {
            break;
        }
    }
}

