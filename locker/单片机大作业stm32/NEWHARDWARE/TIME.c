/**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407Ƕ��ʽʵ����
**	  Gpio�����ļ�
**
**-------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "TIME.h"
#include "led.h"
#include "lcd.h"
/**********************************************************************************************************
�������ƣ���ʱ����ʼ������
�����������
�����������
��ʱ����TIM2
**********************************************************************************************************/
extern u8 error_count;
u8 led_light=0;
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);


    /* Timer2�ж�*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* ��������*/
    TIM_TimeBaseStructure.TIM_Period = 400;							//	����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;						//	Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//	���ϼ���

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /*ʹ��Ԥװ��*/
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    /*Ԥ����������ж�λ*/
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

    /* ��������ж�*/
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);

    /* ����TIM2��ʼ���� */
    TIM_Cmd(TIM2, DISABLE);
}
int flag=1;
int wechat=0;
extern int WIFI_state;
void TIM2_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
				if(WIFI_state==1)
				{
					LCD_ShowString_1(380,2,"����δ����");
				}
				else if(WIFI_state==2)
				{
					LCD_ShowString_1(380,2,"����������");
				}
				if(error_count>=5)
				{
        LED1_REVERSE;
        LED2_REVERSE;
        LED3_REVERSE;
        LED4_REVERSE;
				LED5_REVERSE;
        LED6_REVERSE;
        LED7_REVERSE;
        LED8_REVERSE;
				led_light+=1;
				if(wechat==0)
				{
					wechat=1;
					USART_SendData(UART5,0X31);
				}
				if(led_light>10)
				{
					led_light=0;
					error_count=0;
					wechat=0;
					LED1_OFF;
					LED2_OFF;
					LED3_OFF;
					LED4_OFF;
					LED5_OFF;
					LED6_OFF;
					LED7_OFF;
					LED8_OFF;
					LCD_ShowString_1(100,400,"               ");
					//LCD_Fill(120,300);
				}
				
				}

	}
	
}
///TIM1 ���
void TIM1_Configuration(void)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	//���ò���д������,�������
	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource8|GPIO_PinSource11,GPIO_AF_TIM1); //GPIO����Ϊ��ʱ��1
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); //����GPIOA_Pin8ΪTIM1_Ch1, 
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);//����GPIOA_Pin11ΪTIM1_Ch4,ע��û��CH4N 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��P
	
    TIM_TimeBaseStructure.TIM_Period=20000 - 1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=168 - 1;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//Ĭ�Ͼ�Ϊ0
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
 
	//��ʼ��TIM1  PWMģʽ	 
	//PWM ģʽ 1�C�C �ڵ�������ģʽ�£�ֻҪ TIMx_CNT<TIMx_CCR1��ͨ�� 1 ��Ϊ��Ч״̬������Ϊ��Ч״̬���ڵݼ�����ģʽ�£�
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�,��Ч��ƽΪ�͵�ƽ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;//�ڿ���ʱ���     ��,��������ÿ��Ըı�TIM_OCPolarity ���û��䣬��1ͨ��������
	TIM_OCInitStructure.TIM_Pulse = 500; //����ͨ��1 CCR1��ռ�ձ���ֵ��
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1��ʼ��
	
//	TIM_OCInitStructure.TIM_Pulse = 5000;
//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//ͨ��4
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���,CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��TIM1��PWM�����TIM1��TIM8��Ч,���û�����л�����
} 
void open_door()
{
	TIM_SetCompare1(TIM1,1800);
	TIM_SetCompare2(TIM1,1800);
	TIM_SetCompare3(TIM1,1800);
	TIM_SetCompare4(TIM1,1800);
	delay_ms(1000);
	TIM_SetCompare1(TIM1,800);
	TIM_SetCompare2(TIM1,800);
	TIM_SetCompare3(TIM1,800);
	TIM_SetCompare4(TIM1,800);
}
//void TIM1_IRQHandler(void)
//{
//	
//	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
//	{
//		        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
//        LED1_ON;
//        

//	}	
//}
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);


    /* Timer2�ж�*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* ��������*/
    TIM_TimeBaseStructure.TIM_Period = 400;							//	����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;						//	Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//	���ϼ���

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /*ʹ��Ԥװ��*/
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /*Ԥ����������ж�λ*/
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

    /* ��������ж�*/
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);

    /* ����TIM2��ʼ���� */
    TIM_Cmd(TIM3, DISABLE);
}
void TIM3_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		
	}

}