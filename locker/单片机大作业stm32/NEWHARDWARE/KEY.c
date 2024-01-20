/**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407Ƕ��ʽʵ����
**	  Gpio�����ļ�
**
**-------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "KEY.h"
#include "delay.h"
#include "lcd.h"
#include "touch.h"
#include "led.h"
#include "w25qxx.h" 
#include "spi.h"
extern u8* key_menu[15];
char a[10];//��¼�������
u8* a_shadow[10]={" ","*","**","***","****","*****","******","*******","********","*********"};
char temp[10];
char error_str[20];
char secret[10]={'1','1','1','1','1'};//���룬����ʵ��flash��д
unsigned char secret_len[1]={'5'};
unsigned char n=0;
u8 error_count=0;
int Int_flag=0;
extern uint8_t UID[4],Temp[4];
extern uint8_t UI4[4];							//��Ƭ0ID����
extern uint8_t UI1[4];							//��Ƭ1ID����
extern uint8_t UI2[4];							//��Ƭ2ID����
extern uint8_t UI3[4];							//��Ƭ3ID����
extern int shadow_mode;
/******************************************************************************************
*�������ƣ�void KEYGpio_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����KEY��ʼ��
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
void EXTI_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   //  ʹ��GPIOFʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  //  ʹ��SYSCFGʱ��

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;            //  ����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //  ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;               //  S1��Ӧ����
    GPIO_Init(GPIOF, &GPIO_InitStructure);                  //  ��ʼ��GPIO 

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);   //  ��ʼ���ж���0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);   //  ��ʼ���ж���0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource10);   //  ��ʼ���ж���0

	
    EXTI_InitStructure.EXTI_Line = EXTI_Line8|EXTI_Line9|EXTI_Line10;              //  �����ж���Ϊ�ж���8
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //  �����ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //  ����Ϊ�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //  �����ж���ʹ��
    EXTI_Init(&EXTI_InitStructure);                         //  ����

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;      //�ⲿ�ж� 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;    //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;   //��Ӧ���ȼ�    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                         //���� NVIC
		
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;      //�ⲿ�ж� 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;    //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;   //��Ӧ���ȼ�    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                         //���� NVIC

}
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        Int_flag = 1;

        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        Int_flag = 2;

        EXTI_ClearITPendingBit(EXTI_Line9);
    }

}
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        Int_flag = 3;

        EXTI_ClearITPendingBit(EXTI_Line10);
    }


}
/**********************************************************************************************************
�������ƣ�����ɨ�躯��
�����������
�����������
�������أ�����ֵ
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
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //ͨ�����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //������������
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //��ʼ�� GPIO
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
void bi()//��һ��
{
	bibi_start();
	delay_ms(8);
	bibi_stop();
	delay_ms(8);
}
//LCD Key
void Load_keyboard(u16 x,u16 y,u8 **key_content)
{
	u16 i;
	POINT_COLOR=BLACK; 
//	keycontent=key_content;
	LCD_Fill(x,y,x+240,y+150,WHITE);
	LCD_DrawRectangle(x,y,x+479,y+299);						   
	LCD_DrawRectangle(x+160,y,x+320,y+299);	 
	LCD_DrawRectangle(x,y+60,x+479,y+240);
	LCD_DrawRectangle(x,y+120,x+479,y+180);
	POINT_COLOR=BLACK;
	for(i=0;i<15;i++)
	{
		//if(i==1) //"��"����Ҫ�м���ʾ
			LCD_ShowString(x+(i%3)*160+60,y+20+60*(i/3),48,30,16,key_menu[i]);	
		//else
			//Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),key_menu[i],16,80);
	} 
}
u8 compare_key()
{
	u8 i=0;
	u8 j=0;
	bi();
	if(n!=secret_len[0])//����Ҫ��
	{
		error_count+=1;
		for(j=0;j<n;j++)
		{
			a[j]=' ';
		}
		if(error_count==1)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��4�λ���");	
		}
		else if(error_count==2)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��3�λ���");
		}
		else if(error_count==3)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��2�λ���");
		}
		else if(error_count==4)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��1�λ���");
		}
		else if(error_count==5)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��0�λ���");
		}
		
		n=0;
		return 0;
	}
	for(i=0;i<secret_len[0];i++)//����Ҫ��
	{
		if(a[i]!=secret[i])
		{
			error_count+=1;
			for(j=0;j<n;j++)
			{
				a[j]=' ';
						
			}
		if(error_count==1)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��4�λ���");	
		}
		else if(error_count==2)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��3�λ���");
		}
		else if(error_count==3)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��2�λ���");
		}
		else if(error_count==4)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��1�λ���");
		}
		else if(error_count==5)
		{
			LCD_ShowString_1(180,300,"�㻹ʣ��0�λ���");
		}
			n=0;
			return 0;
		}


	}
	bi();
	error_count=0;
	for(j=0;j<10;j++)
	{
				a[j]=' ';
//				a_shadow[j]=' ';
	}	
	n=0;
	LCD_ShowString(200, 200, 240, 24, 24, a);	
	LCD_ShowString_1(180,300,"    ������ȷ     ");
	delay_ms(500);
	LCD_ShowString_1(180,300,"   ����������     ");


	return 1;
}
void Key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>16)return;
	if(sta )  //"��"���򲻲���&&keyx!=1
		LCD_Fill(x+j*160+2,y+i*60+2,x+j*160+158,y+i*60+58,GRAY);
	else //if(keyx!=1)  //���ֺ�����ָ���ɫ 
	{
		LCD_Fill(x+j*160+2,y+i*60+2,x+j*160+158,y+i*60+58,WHITE);
		LCD_ShowString(x+j*160+60,y+20+60*i,48,30,16,key_menu[keyx]);	
	}
}
//
void reset_a()
{
	int i;
	n=0;
	for(i=0;i<10;i++)        //�������
	{
	a[i]=' ';
//	a_shadow[i]=' ';
	}
}
// 160 300
u8 key_state=0;
//////������������
u8 Get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;
	u8 key=0;
	tp_dev.scan(0); 		
						if(n>=8)
					{
						n=0;
						for(i=0;i<10;i++)        //�������
						{
							a[i]=' ';
//						  a_shadow[i]=' ';
							//n=1;
						}
					}
	if(tp_dev.sta&TP_PRES_DOWN)  //������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*160+160)&&tp_dev.x[0]>(x+j*160)&&tp_dev.y[0]<(y+i*60+60)&&tp_dev.y[0]>(y+i*60))  //����ÿһ�������������ֵ
				{	
					key=i*3+j+1;	//i������ ÿһ�еı仯��Ҫ�仯����ֵ j������ ÿһ�б仯����Ҫ�仯һ��ֵ
					if(key_state==0)
					{
					if(key==1)
					{
//					a_shadow[n]='*';	
					a[n++]='1';
//						LED1_REVERSE;
					}
					if(key==2)
					{
//					a_shadow[n]='*';	
					a[n++]='2';			
//						LED2_REVERSE;
					}
					if(key==3)
					{
//					a_shadow[n]='*';	
					a[n++]='3';
//						bibi_start();
					}
					if(key==4)
					{
//					a_shadow[n]='*';	
					a[n++]='4';	
//						bibi_stop();
					}
					if(key==5)
					{
//					a_shadow[n]='*';	
					a[n++]='5';
					}
					if(key==6)
					{
//					a_shadow[n]='*';	
					a[n++]='6';
					}
					if(key==7)
					{
//					a_shadow[n]='*';	
					a[n++]='7';
					}
					if(key==8)
					{
//					a_shadow[n]='*';	
					a[n++]='8';
					
					}
					if(key==9)
					{
//					a_shadow[n]='*';
					a[n++]='9';
					
					}
					if(key==10)
					{
//					a_shadow[n]='*';	
					a[n++]='0';
					
					}
					if(key==11)
					{
					n--;
					a[n]=' ';
//					a_shadow[n]=' ';
					}
					if(key==12)
					{
					compare_key();
//					LCD_ShowNum(0,0,error_count,24,24);
					}
					if(key==13)
					{
						if(shadow_mode==1)shadow_mode=0;
						else shadow_mode=1;
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
					Key_staset(x,y,key_x-1,0);  
					key_x=key;
					Key_staset(x,y,key_x-1,1);  //������һ����������������
				}
				break;
			}
	  }  
		//while (!(tp_dev.sta));
	}
	else if(key_x)  //ÿ�δ�0��ʼ ����¼�ϴε�״̬
	{
		Key_staset(x,y,key_x-1,0);
		key_x=0;
		key_state=0;
	} 
//	    USART_SendData(USART3,key);
	return key; 
}
//////���ڴ洢�޸ĺ�����
u8 Get_keynum1(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;
	u8 key=0;
	tp_dev.scan(0); 		
						if(n>=8)
					{
						n=0;
						for(i=0;i<9;i++)        //�������
						{
							a[i]=' ';
//							a_shadow[i]=' ';
							//n=1;
						}
					}
	if(tp_dev.sta&TP_PRES_DOWN)  //������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*160+160)&&tp_dev.x[0]>(x+j*160)&&tp_dev.y[0]<(y+i*60+60)&&tp_dev.y[0]>(y+i*60))  //����ÿһ�������������ֵ
				{	
					key=i*3+j+1;	//i������ ÿһ�еı仯��Ҫ�仯����ֵ j������ ÿһ�б仯����Ҫ�仯һ��ֵ
					if(key_state==0)
					{
					if(key==1)
					{
					a[n++]='1';
//					a_shadow[n++]='*';
//						LED1_REVERSE;
					}
					if(key==2)
					{
					a[n++]='2';
//					a_shadow[n++]='*';
//						LED2_REVERSE;
					}
					if(key==3)
					{
					a[n++]='3';
//					a_shadow[n++]='*';
//						bibi_start();
					}
					if(key==4)
					{
					a[n++]='4';
//					a_shadow[n++]='*';
//						bibi_stop();
					}
					if(key==5)
					{
					a[n++]='5';
//					a_shadow[n++]='*';
					}
					if(key==6)
					{
					a[n++]='6';
//					a_shadow[n++]='*';
					}
					if(key==7)
					{
					a[n++]='7';
//					a_shadow[n++]='*';
					}
					if(key==8)
					{
					a[n++]='8';
//					a_shadow[n++]='*';
					}
					if(key==9)
					{
					a[n++]='9';
//					a_shadow[n++]='*';
					}
					if(key==10)
					{
					a[n++]='0';
//					a_shadow[n++]='*';
					}
					if(key==11)
					{
					n--;
					a[n]=' ';
//					a_shadow[n]=' ';
					}
					if(key==12)
					{
						if(n>0)
					 {
						bi();
						LCD_ShowString_1(180,300," ¼��ɹ� ");
					 W25QXX_Erase_Sector(0x000000);
					 W25QXX_Write_NoCheck((u8*)a, 0x000000, n);
					 W25QXX_Read((u8*)secret, 0x000000, n);
					 secret_len[0]=n;	 
					 W25QXX_Write_NoCheck((u8*)secret_len, 0x000010, 1);
					 W25QXX_Read((u8*)secret_len, 0x000010, 1);
//LCD_ShowNum(0,0,secret[0],24,24);
//LCD_ShowNum(0,24,secret[1],24,24);
//LCD_ShowNum(0,48,secret[2],24,24);
//LCD_ShowNum(0,72,secret[3],24,24);
//LCD_ShowNum(0,96,secret[4],24,24);
//LCD_ShowString(200, 300, 240, 24, 24, temp);
//LCD_ShowString(200, 400, 240, 24, 24, temp);
						delay_ms(10);
						LCD_ShowString_1(180,300,"             ");
					 }
					}
					if(key==13)
					{
						if(shadow_mode==1)shadow_mode=0;
						else shadow_mode=1;
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
					Key_staset(x,y,key_x-1,0);  
					key_x=key;
					Key_staset(x,y,key_x-1,1);  //������һ����������������
				}
				break;
			}
	  }  
		//while (!(tp_dev.sta));
	}
	else if(key_x)  //ÿ�δ�0��ʼ ����¼�ϴε�״̬
	{
		Key_staset(x,y,key_x-1,0);
		key_x=0;
		key_state=0;
	} 
//	    USART_SendData(USART3,key);
	return key; 
}
void get_information()
{
	W25QXX_Read((u8*)secret_len, 0x000010, 1);
	W25QXX_Read((u8*)secret, 0x000000, secret_len[0]);
	
}

void remove_cardload()
{
	LCD_ShowString_1(200,50," ɾ��ģʽ ");
	LCD_DrawRectangle(100,100,360,220);						   
	LCD_DrawRectangle(100,250,360,370);		
	LCD_DrawRectangle(100,400,360,520);						   
	LCD_DrawRectangle(100,550,360,670);	
	if(UI1[0] == 0xFF && UI1[1] == 0xFF && UI1[2] == 0xFF && UI1[3] == 0xFF){LCD_ShowString_1(200,150," �������� ");}
	else {LCD_ShowString_1(200,150,card_menu[0]);}
	if(UI2[0] == 0xFF && UI2[1] == 0xFF && UI2[2] == 0xFF && UI2[3] == 0xFF){LCD_ShowString_1(200,300," �������� ");}
	else {LCD_ShowString_1(200,300,card_menu[1]);}
	if(UI3[0] == 0xFF && UI3[1] == 0xFF && UI3[2] == 0xFF && UI3[3] == 0xFF){LCD_ShowString_1(200,450," �������� ");}
	else {LCD_ShowString_1(200,450,card_menu[2]);}
	if(UI4[0] == 0xFF && UI4[1] == 0xFF && UI4[2] == 0xFF && UI4[3] == 0xFF){LCD_ShowString_1(200,600," �������� ");}
	else {LCD_ShowString_1(200,600,card_menu[3]);}
}
void card_staset(u8 keyx,u8 sta)
{		  
	u16 i=keyx;
	if(keyx>16)return;
	if(sta)  //
	{
		LCD_Fill(102,keyx*150+101,358,keyx*150+219,GRAY);
	}
	else //if(keyx!=1)  //���ֺ�����ָ���ɫ 
	{
		LCD_Fill(102,keyx*150+101,358,keyx*150+219,WHITE);
		LCD_ShowString_1(200,150*keyx+150," �������� ");
	}
}
u8 remove_card()
{
	u16 i,j;
	static u8 key_x=0;
	u8 key=0;
	tp_dev.scan(0); 		
	if(tp_dev.sta&TP_PRES_DOWN)  //������������
	{	
		for(i=0;i<4;i++)
		{
			if(tp_dev.x[0]<(360)&&tp_dev.x[0]>(100)&&tp_dev.y[0]<(150*i+220)&&tp_dev.y[0]>(150*i+100))
			{
				if(key_state==0)
				{
				key=i+1;
				//LED1_REVERSE;
				if(key==1)
				{
						W25QXX_Erase_Sector(0x000001);
						UI1[0]=0xFF;
						UI1[1]=0xFF;
						UI1[2]=0xFF;
						UI1[3]=0xFF;
//LCD_ShowString_1(200,150," �������� ");


				}else if(key==2)
				{
						W25QXX_Erase_Sector(0x000002);
						UI2[0]=0xFF;
						UI2[1]=0xFF;
						UI2[2]=0xFF;
						UI2[3]=0xFF;//LCD_ShowString_1(200,300," �������� ");
				}
				else if(key==3)
				{
						W25QXX_Erase_Sector(0x000003);
						UI3[0]=0xFF;
						UI3[1]=0xFF;
						UI3[2]=0xFF;
						UI3[3]=0xFF;//LCD_ShowString_1(200,450," �������� ");
				}
				else if(key==4)
				{
						W25QXX_Erase_Sector(0x000004);
						UI4[0]=0xFF;
						UI4[1]=0xFF;
						UI4[2]=0xFF;
						UI4[3]=0xFF;
				//LCD_ShowString_1(200,600," �������� ");
				}
				}
				key_state=1;
				//break;
			}
			if(key) //key!=0
			{	   
				if(key_x==key)key=0; 
				else 
				{
					card_staset(key_x-1,0);  
					key_x=key;
					card_staset(key_x-1,1);  //������һ����������������
				}
				break;
			}
		}
	}
	else if(key_x)  //ÿ�δ�0��ʼ ����¼�ϴε�״̬
	{
		card_staset(key_x-1,0);
		key_x=0;
		key_state=0;
	} 
//	    USART_SendData(USART3,key);
	return key; 
}
