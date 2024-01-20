#ifndef __LED_H
#define __LED_H	 

#include "sys.h" 

//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 
#define  	LED1_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_0)
#define  	LED1_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_0)
#define  	LED1_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_0, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_0))

//LED2      PF1
#define  	LED2_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_1)
#define  	LED2_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_1)
#define  	LED2_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_1, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1))

//LED3      PF2
#define  	LED3_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_2)
#define  	LED3_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_2)
#define  	LED3_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_2, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_2))

//LED4      PF3
#define  	LED4_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_3)
#define  	LED4_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_3)
#define  	LED4_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_3, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_3))

//LED5      PF4
#define  	LED5_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_4)
#define  	LED5_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_4)
#define  	LED5_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_4, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_4))

//LED6      PF5
#define  	LED6_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_5)
#define  	LED6_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_5)
#define  	LED6_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_5, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_5))

//LED7      PF6
#define  	LED7_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_6)
#define  	LED7_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_6)
#define  	LED7_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_6, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_6))

//LED8      PF7
#define  	LED8_ON      	GPIO_ResetBits(GPIOF, GPIO_Pin_7)
#define  	LED8_OFF      	GPIO_SetBits(GPIOF, GPIO_Pin_7)
#define  	LED8_REVERSE  	GPIO_WriteBit(GPIOF, GPIO_Pin_7, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_7))


#define  	BIBI_REVERSE  	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)!GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
#define  	BIBI_OFF      		GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define  	LED8_ON      	GPIO_SetBits(GPIOC, GPIO_Pin_13)


void LED_Init(void);//初始化		 				    
#endif

















