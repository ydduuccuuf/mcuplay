#include "led.h" 

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  LED
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//LED��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //ͨ�����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //������������
    GPIO_Init(GPIOF, &GPIO_InitStructure);                  //��ʼ�� GPIO
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
		LED5_OFF;
    LED6_OFF;
    LED7_OFF;
    LED8_OFF;
}





