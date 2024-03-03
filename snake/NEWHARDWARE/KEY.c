/**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407嵌入式实验箱
**	  Gpio配置文件
**
**-------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "KEY.h"
#include "delay.h"
#include "lcd.h"
#include "touch.h"
#include "picture1.h"
extern u8* key_menu[15];
char a[10];
char secret[5]={'1','1','1','1','1'};
unsigned char n=0;
u8 error_count=0;
u8 exit_keyboard = 0;
/******************************************************************************************
*函数名称：void KEYGpio_Init(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：KEY初始化
//KEY_S1      PF8
//KEY_S2      PF9
//KEY_S3      PF10
*******************************************************************************************/
void KEYGpio_Init(void)
{    
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  KEY_S1  	PF8
	//  KEY_S2      PF9
	//  KEY_S3      PF10
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/**********************************************************************************************************
函数名称：按键扫描函数
输入参数：无
输出参数：无
函数返回：按键值
**********************************************************************************************************/
unsigned char KeyScan(void)
{		 
    unsigned char buf[4] = {0};
    unsigned char temp = 0;
    static u8 key_up=1;
    
    buf[0] = KEY_S1_READ();
    buf[1] = KEY_S2_READ();
	buf[2] = KEY_S3_READ();
    
    if(key_up && (buf[0] == 0 || buf[1] == 0 || buf[2] == 0))
    {
        key_up = 0;
        
        delay_ms(100);
    
        buf[0] = KEY_S1_READ();
		buf[1] = KEY_S2_READ();
		buf[2] = KEY_S3_READ();
        
        //  KEY_S1
        if ( (buf[0] == 0) && (buf[1] == 1) && (buf[2] == 1))
        {
            temp = 1;
        }
        
        //  KEY_S2
        if ( (buf[0] == 1) && (buf[1] == 0) && (buf[2] == 1))
        {
            temp = 2;
        }
		
		//  KEY_S3
        if ( (buf[0] == 1) && (buf[1] == 1) && (buf[2] == 0))
        {
            temp = 3;
        }
    }
    else if (buf[0] == 1 && buf[1] == 1 && buf[2] == 1) key_up = 1;
    
    return temp;
}
//bee
void bibi_init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //通用输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //输出推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉或下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //初始化 GPIO
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
void bibi_start()
{
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
void bibi_stop()
{

	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
//LCD Key

void Load_keyboard(u16 x,u16 y,u8 **key_content)
{
	u16 i;
	POINT_COLOR=BLACK; //字体颜色为红色
//	keycontent=key_content;
	LCD_Fill(x,y,x+240,y+150,WHITE);
	LCD_DrawRectangle(x,y,x+479,y+299);						   
	LCD_DrawRectangle(x+160,y,x+320,y+299);	 
	LCD_DrawRectangle(x,y+60,x+479,y+240);
	LCD_DrawRectangle(x,y+120,x+479,y+180);
	POINT_COLOR=BLACK;
	for(i=0;i<15;i++)
	{
		//if(i==1) //"："不需要中间显示
			LCD_ShowString(x+(i%3)*160+60,y+20+60*(i/3),48,30,16,key_menu[i]);	
		//else
			//Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),key_menu[i],16,80);
	} 
}
int right_flag=0;
u8 compare_key()
{
	u8 i=0;
	u8 j=0;
	if(n!=5)
	{
		error_count+=1;
		for(j=0;j<n;j++)
		{
			a[j]=' ';
		}
					LCD_ShowString(100,400,24,24,24,"N");
			n=0;
		return 0;
	}
	for(i=0;i<5;i++)
	{
		if(a[i]!=secret[i])
		{
			error_count+=1;
			for(j=0;j<n;j++)
			{
				a[j]=' ';
						
			}
				LCD_ShowString(100,400,24,24,24,"N");
			n=0;
			return 0;
		}


	}
	error_count=0;
	LCD_ShowString(100,400,24,24,24,"Y");
	right_flag=1;
	for(j=0;j<n;j++)
	{
				a[j]=' ';
	}
	n=0;	
	return 0;


}

// 160 300
u8 key_state=0;
u8 Get_keynum(u16 x,u16 y)
{    
	if (exit_keyboard) {
        return 0;
    }
	u16 i,j;
	static u8 key_x=0;
	u8 key=0;
	tp_dev.scan(0); 		
						if(n>=8)
					{
						n=0;
						for(i=0;i<9;i++)        //清空数组
						{
							a[i]=' ';
							//n=1;
						}
					}
	if(tp_dev.sta&TP_PRES_DOWN)  //触摸屏被按下
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*160+160)&&tp_dev.x[0]>(x+j*160)&&tp_dev.y[0]<(y+i*60+60)&&tp_dev.y[0]>(y+i*60))
				{	
					key=i*3+j+1;
					if(key_state==0)
					{
					if(key==1)
					{
					a[n++]='1';
					}
					if(key==2)
					{
					a[n++]='2';
					}
					if(key==3)
					{
					a[n++]='3';
					}
					if(key==4)
					{
					a[n++]='4';
					}
					if(key==5)
					{
					a[n++]='5';
					}
					if(key==6)
					{
					a[n++]='6';
					}
					if(key==7)
					{
					a[n++]='7';
					}
					if(key==8)
					{
					a[n++]='8';
					}
					if(key==9)
					{
					a[n++]='9';
					}
					if(key==10)
					{
					a[n++]='0';
					}
					if(key==11)
					{
					n--;
					a[n]=' ';
					}
					if(key==12)
					{
					compare_key();
					LCD_ShowNum(0,0,error_count,24,24);
					}
				}
					key_state=1;
					
		
					break;	 		   
				}
			}

			if(key) //key!=0
			{	   
				if(key_x==key)key=0; 
				else 
				{
					
					key_x=key;
				}
				break;
			}
	  }  
		//while (!(tp_dev.sta));
	}
	else if(key_x)  //每次从0开始 不记录上次的状态
	{
		key_x=0;
		key_state=0;
	} 
//	if (compare_key() == 1) {
//		LCD_Clear(WHITE);
//    display_pattern();
//}//	    USART_SendData(USART3,key);
	return key; 

}

