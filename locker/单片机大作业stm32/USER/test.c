#include "sys.h"
#include "delay.h"
#include "USART.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"
#include "KEY.h"
#include "USART.h"
#include "TIME.h"
#include "RC522.h"
#include "w25qxx.h" 
#include "spi.h"
//清空屏幕并在右上角显示"RST"
uint8_t cardnumber,KeyNum,tempcard,select=0,flag_scan=1,flag_addcard=0,flag_deletecard=0;
extern uint8_t UID[4],Temp[4];
extern uint8_t UI4[4];
extern uint8_t UI1[4];
extern uint8_t UI2[4];
extern uint8_t UI3[4];
extern Int_flag;
extern u8* a_shadow[10];
extern char secret[5];
extern unsigned char secret_len[1];
int shadow_mode=0;
int mode=1;
void RFID_Check(void);
void Read_Card(void);
extern int n;
int usart_door=0;
void Load_Drow_Dialog(void)
{
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = BLUE; //设置字体为蓝色
//    LCD_ShowString(lcddev.width - 50, 20, 200, 24, 24, "RST"); //显示清屏区域
    POINT_COLOR = RED; //设置画笔红色
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color)
{
    if(len == 0)return;
    LCD_Fill(x0, y0, x0 + len - 1, y0, color);
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0, u16 y0, u16 r, u16 color)
{
    u32 i;
    u32 imax = ((u32)r * 707) / 1000 + 1;
    u32 sqmax = (u32)r * (u32)r + (u32)r / 2;
    u32 x = r;
    gui_draw_hline(x0 - r, y0, 2 * r, color);
    for (i = 1; i <= imax; i++)
    {
        if ((i * i + x * x) > sqmax) // draw lines from outside
        {
            if (x > imax)
            {
                gui_draw_hline (x0 - i + 1, y0 + x, 2 * (i - 1), color);
                gui_draw_hline (x0 - i + 1, y0 - x, 2 * (i - 1), color);
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline(x0 - x, y0 + i, 2 * x, color);
        gui_draw_hline(x0 - x, y0 - i, 2 * x, color);
    }
}
//两个数之差的绝对值
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1, u16 x2)
{
    if(x1 > x2)return x1 - x2;
    else return x2 - x1;
}
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    if(x1 < size || x2 < size || y1 < size || y2 < size)return;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if(delta_x > 0)incx = 1; //设置单步方向
    else if(delta_x == 0)incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if(delta_y > 0)incy = 1;
    else if(delta_y == 0)incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if( delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for(t = 0; t <= distance + 1; t++ ) //画线输出
    {
        gui_fill_circle(uRow, uCol, size, color); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

const u16 POINT_COLOR_TBL[5] = {RED, GREEN, BLUE, BROWN, GRED};
//电容触摸屏测试函数
void ctp_test(void)
{
    u8 t = 0;
    u8 i = 0;
    u16 lastpos[5][2];		//最后一次的数据
    while(1)
    {
        tp_dev.scan(0);

        for(t = 0; t < 5; t++)
        {
            if((tp_dev.sta) & (1 << t))
            {
                if(tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)
                {
                    if(lastpos[t][0] == 0XFFFF)
                    {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }
                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]); //画线
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];

                    if(tp_dev.x[t] > (lcddev.width - 50) && tp_dev.y[t] < 50)
                    {
                        Load_Drow_Dialog();//清除
                    }
                }
            }
            else
            {
                lastpos[t][0] = 0XFFFF;
            }
        }

        delay_ms(5);
        i++;
        if(i % 20 == 0)LED0 = !LED0;
    }
}
//读卡函数，读卡并获取卡编号
void RFID_Check()									
{
	cardnumber = Rc522Test();	//获取卡编号
	if(cardnumber == 0)			//如果为0，表示“卡片错误”，系统中没有这张卡
	{

		bibi_start();
		WaitCardOff();
		bibi_stop();
	}
	else if(cardnumber==1||cardnumber==2||cardnumber==3||cardnumber == 4)			//如果卡编号为1-4，说明是系统中的4张卡
	{	
		bi();
		LCD_ShowString_1(180,300,"    密码正确     ");
		WaitCardOff();		//等待卡片移开
		delay_ms(100);
		LCD_ShowString_1(180,300,"   请输入密码     ");
	}	
}

//从flash中读取各卡信息
void Read_Card()
{
	W25QXX_Read((u8*)UI1, 0x001000, 4);
	W25QXX_Read((u8*)UI2, 0x002000, 4);
	W25QXX_Read((u8*)UI3, 0x003000, 4);
	W25QXX_Read((u8*)UI4, 0x004000, 4);
}
extern u8* key_menu[15];
extern char a[10];
char b[2]={'a','b'};

int i=0;
unsigned char WriteBuf[257] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
unsigned char ReadBuf[256]  = {0};
unsigned char ReadBuf2[1]  = {0};
#define SIZE sizeof(WriteBuf)

int main(void)
{
	    volatile unsigned short temp = 0;  
	    volatile unsigned char value = 0;
	uint8_t cardno;
		u8 m=0;
		u16 x=0;
		u16 y=0;
		UART5_Configuration(115200);
		W25QXX_Init();
		TIM2_Configuration();

		RFID_Init();
    delay_init(168);			//延时初始化
		bibi_init();

		UART3_Configuration();
    LCD_Init();					//LCD初始化
		LED_Init();
    tp_dev.init();				//触摸屏初始化
//		TIM1_Configuration();	
    POINT_COLOR = RED;          //设置字体为红色
    LCD_ShowString(180, 50,  200, 24, 24, "MK-407DK");
    LCD_ShowString(30, 80,  200, 16, 16, "TOUCH TEST");
    LCD_ShowString(30, 100, 200, 16, 16, "2022/01/10");
    delay_ms(200);
//W25QXX_Erase_Sector(0x000001);
//W25QXX_Erase_Sector(0x000002);
//W25QXX_Erase_Sector(0x000003);
//W25QXX_Erase_Sector(0x000004);
    Load_Drow_Dialog();
		get_information();
		Read_Card();
		USART_Cmd(UART5, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
				EXTI_Config();
//	LCD_ShowNum(0,0,secret[0],24,24);
//	LCD_ShowNum(0,24,secret[1],24,24);
//	LCD_ShowNum(0,48,secret[2],24,24);
//	LCD_ShowNum(0,72,secret[3],24,24);
//	LCD_ShowNum(0,96,secret[4],24,24);
//	LCD_ShowNum(0,120,secret[5],24,24);
//	LCD_ShowNum(0,144,secret_len[0],24,24);		
		Load_keyboard(0,500,key_menu);
//	LCD_ShowNum(0,0,UI4[0],24,24);
//	LCD_ShowNum(0,24,UI4[1],24,24);
//	LCD_ShowNum(0,48,UI4[2],24,24);
//	LCD_ShowNum(0,72,UI4[3],24,24);
//	LCD_ShowNum(0,96,secret[4],24,24);
//open_door();
	while(1)
	{
			if(Int_flag==1)//默认为锁
			{
				mode=1;
				delay_ms(2);
				Int_flag=0;
			}
			else if(Int_flag==2)//重设密码
			{
				mode=2;
				delay_ms(2);
				Int_flag=0;
			}
			else if(Int_flag==3)//录卡
			{
				mode=3;
				delay_ms(2);
				Int_flag=0;
			}
			
			if(mode==1)//默认为锁
			{
			LCD_Clear(WHITE);//清屏
			delay_ms(1);
			Load_keyboard(0,500,key_menu);
			reset_a();
			while(Int_flag==0||Int_flag==1)
			{
			LCD_ShowString_1(200,50," 密码模式 ");
			Get_keynum(0,500);
			if(shadow_mode==0) {LCD_ShowString(200, 200, 240, 24, 24, a);}
//			else {LCD_ShowString(200, 200, 240, 24, 24, a_shadow[n]);}
			RFID_Check();
			if(usart_door==1)
			{
				LCD_ShowString_1(180,300,"  通过手机解锁  ");
				bi();
				delay_ms(200);
				usart_door=0;
				LCD_ShowString_1(180,300,"                ");
			}
			}
			LCD_ShowString_1(180,300,"             ");
			}
			else if(mode==2)//重设密码
			{
			LCD_Clear(WHITE);//清屏
			delay_ms(1);
			Load_keyboard(0,500,key_menu);
			reset_a();
			while(Int_flag==0||Int_flag==2)
			{
				Get_keynum1(0,500);
			if(shadow_mode==0){LCD_ShowString(200, 200, 240, 24, 24, a);}
//			else {LCD_ShowString(200, 200, 240, 24, 24, a_shadow[n]);}
				LCD_ShowString_1(200,50," 重设密码 ");
			if (PcdRequest(REQ_ALL, Temp) == MI_OK)
			{
			if (PcdAnticoll(UID) == MI_OK)
			{  
			if(UI1[0] == 0xFF && UI1[1] == 0xFF && UI1[2] == 0xFF && UI1[3] == 0xFF) tempcard = 1;
			else if(UI2[0] == 0xFF && UI2[1] == 0xFF && UI2[2] == 0xFF && UI2[3] == 0xFF) tempcard = 2;
			else if(UI3[0] == 0xFF && UI3[1] == 0xFF && UI3[2] == 0xFF && UI3[3] == 0xFF) tempcard = 3;
			else if(UI4[0] == 0xFF && UI4[1] == 0xFF && UI4[2] == 0xFF && UI4[3] == 0xFF) tempcard = 4;
			else tempcard = 4;
			//防止重复录卡
			if(UID[0]==UI4[0]&&UID[1]==UI4[1]&&UID[2]==UI4[2]&&UID[3]==UI4[3])cardno=4;
			else if(UID[0]==UI1[0]&&UID[1]==UI1[1]&&UID[2]==UI1[2]&&UID[3]==UI1[3])cardno=1;
			else if(UID[0]==UI2[0]&&UID[1]==UI2[1]&&UID[2]==UI2[2]&&UID[3]==UI2[3])cardno=2;
			else if(UID[0]==UI3[0]&&UID[1]==UI3[1]&&UID[2]==UI3[2]&&UID[3]==UI3[3])cardno=3;
			else cardno = 0;
				if(cardno==0)
				{
				if(tempcard == 1)
				{
							LCD_ShowString_1(180,300," 录入成功 ");
							UI1[0] = UID[0];
							UI1[1] = UID[1];
							UI1[2] = UID[2];
							UI1[3] = UID[3];
							bi();
							W25QXX_Write_NoCheck((u8*)UI1, 0x001000, 4);
							WaitCardOff();
							delay_ms(20);
							LCD_ShowString_1(180,300,"             ");
					
				}
				else if(tempcard == 2)
				{
							LCD_ShowString_1(180,300," 录入成功 ");
							UI2[0] = UID[0];
							UI2[1] = UID[1];
							UI2[2] = UID[2];
							UI2[3] = UID[3];
							bi();
							W25QXX_Write_NoCheck((u8*)UI2, 0x002000, 4);
							WaitCardOff();
							delay_ms(20);
							LCD_ShowString_1(180,300,"             ");

				}
				else if(tempcard == 3)
				{
							LCD_ShowString_1(180,300," 录入成功 ");
							UI3[0] = UID[0];
							UI3[1] = UID[1];
							UI3[2] = UID[2];
							UI3[3] = UID[3];
							bi();
							W25QXX_Write_NoCheck((u8*)UI3, 0x003000, 4);
							WaitCardOff();
							delay_ms(20);
							LCD_ShowString_1(180,300,"             ");
				}
				else if(tempcard == 4)
				{
							LCD_ShowString_1(180,300," 录入成功 ");
							UI4[0] = UID[0];
							UI4[1] = UID[1];
							UI4[2] = UID[2];
							UI4[3] = UID[3];
							bi();
							W25QXX_Write_NoCheck((u8*)UI4, 0x004000, 4);
							WaitCardOff();
					    delay_ms(20);
							LCD_ShowString_1(180,300,"             ");
				}
			}
			}		
			}
			}
			LCD_ShowString_1(180,300,"             ");
			}
			else if(mode==3)//录卡
			{

				LCD_Clear(WHITE);//清屏
				delay_ms(1);
				remove_cardload();
				while(Int_flag==0||Int_flag==3)
				{
					remove_card();
				}
				mode=1;
			}
	}

//    if(tp_dev.touchtype & 0X80)
//    {
//        ctp_test();             //电容屏测试
//    }
}









