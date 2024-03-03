#include "led.h" 

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  LED
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//LED对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //通用输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //输出推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉或下拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);                  //初始化 GPIO
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
		LED5_OFF;
    LED6_OFF;
    LED7_OFF;
    LED8_OFF;
}





