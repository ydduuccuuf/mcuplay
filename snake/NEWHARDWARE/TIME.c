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
u8 count = 0;
//struct snake {//�ߵĲ���
//    int length;          // ????
//    char snakecolor;      // ????
//    uint16_t snakeX[100];     // ???????X??
//    uint16_t snakeY[100];     // ???????Y??
//    int headX;           // ???X??
//    int headY;           // ???Y??
//    int dir;             // ??????
//    int tpdir;           // ????
//    int life;            // ??????
//};

//struct snake mySnake;
//struct food {//ʳ�����
//    uint16_t foodX;
//    uint16_t foodY;
//    uint16_t foodcolor;
//};
//typedef enum {
//    UP,
//    DOWN,
//    LEFT,
//    RIGHT
//} Direction;

//Direction snakeDirection = RIGHT; 
//void TIM2_Int_Init(u16 arr,u16 psc)
//{
//TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//NVIC_InitTypeDef NVIC_InitStructure; 
//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);///'??TIM3???
//TIM_TimeBaseInitStructure.TIM_Period = arr; //?????????
//TIM_TimeBaseInitStructure.TIM_Prescaler=psc;//????????
//TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //???????g?
//TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

//TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//??'??TIM3
// 
// TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //?????????3???????
//TIM_Cmd(TIM3,ENABLE); //'??????3
// 
//NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //?????3???
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //????????1
// NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //???????3
// NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
// NVIC_Init(&NVIC_InitStructure);
// 
//}

//?????3????????
//void TIM2_IRQHandler(void)
//{
// if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //??????
// {
// //LED1=!LED1;//DS1???
//HAL_TIM_PeriodElapsedCallback();
//	 refresh();
// }
// TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//?????????
//}
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
    TIM_TimeBaseStructure.TIM_Period = 3999;							//	����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;						//	Ԥ��Ƶ
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
    TIM_Cmd(TIM2, ENABLE);
}
int flag=1;
void TIM2_IRQHandler(void)
{            
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 

}

            

    }
    


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,ENABLE);
	
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		count++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
