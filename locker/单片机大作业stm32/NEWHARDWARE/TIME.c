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
    TIM_TimeBaseStructure.TIM_Period = 400;							//	计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;						//	预分频
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
					LCD_ShowString_1(380,2,"网络未连接");
				}
				else if(WIFI_state==2)
				{
					LCD_ShowString_1(380,2,"网络已连接");
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
///TIM1 舵机
void TIM1_Configuration(void)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	//复用不能写成下面,会出问题
	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource8|GPIO_PinSource11,GPIO_AF_TIM1); //GPIO复用为定时器1
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); //复用GPIOA_Pin8为TIM1_Ch1, 
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);//复用GPIOA_Pin11为TIM1_Ch4,注意没有CH4N 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化P
	
    TIM_TimeBaseStructure.TIM_Period=20000 - 1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler=168 - 1;  //定时器分频
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//默认就为0
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
 
	//初始化TIM1  PWM模式	 
	//PWM 模式 1–– 在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道 1 便为有效状态，否则为无效状态。在递减计数模式下，
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //PWM1为正常占空比模式，PWM2为反极性模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低,有效电平为低电平
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;//在空闲时输出     低,这里的设置可以改变TIM_OCPolarity 如果没这句，第1通道有问题
	TIM_OCInitStructure.TIM_Pulse = 500; //输入通道1 CCR1（占空比数值）
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1初始化
	
//	TIM_OCInitStructure.TIM_Pulse = 5000;
//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//通道4
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器,CCR自动装载默认也是打开的
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能TIM1的PWM输出，TIM1与TIM8有效,如果没有这行会问题
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


    /* Timer2中断*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 基础设置*/
    TIM_TimeBaseStructure.TIM_Period = 400;							//	计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;						//	预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//	向上计数

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /*使能预装载*/
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /*预先清除所有中断位*/
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

    /* 溢出配置中断*/
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);

    /* 允许TIM2开始计数 */
    TIM_Cmd(TIM3, DISABLE);
}
void TIM3_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		
	}

}