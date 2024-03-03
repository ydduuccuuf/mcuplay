/**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407嵌入式实验箱
**	  Gpio配置文件
**
**-------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "TIME.h"
#include "led.h"
#include "lcd.h"
/**********************************************************************************************************
函数名称：定时器初始化函数
输入参数：无
输出参数：无
定时器：TIM2
**********************************************************************************************************/
extern u8 error_count;
u8 led_light=0;
u8 count = 0;
//struct snake {//蛇的参数
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
//struct food {//食物参数
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


    /* Timer2中断*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* 基础设置*/
    TIM_TimeBaseStructure.TIM_Period = 3999;							//	计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;						//	预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//	向上计数

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /*使能预装载*/
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    /*预先清除所有中断位*/
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

    /* 溢出配置中断*/
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);

    /* 允许TIM2开始计数 */
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,ENABLE);
	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		count++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
