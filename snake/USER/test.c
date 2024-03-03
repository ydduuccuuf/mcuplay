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
#include "picture1.h"
//�����Ļ�������Ͻ���ʾ"RST"
void Load_Drow_Dialog(void)
{
    LCD_Clear(WHITE);//����
    POINT_COLOR = BLUE; //��������Ϊ��ɫ
    LCD_ShowString(lcddev.width - 50, 20, 200, 24, 24, "RST"); //��ʾ��������
    POINT_COLOR = RED; //���û��ʺ�ɫ
}
////////////////////////////////////////////////////////////////////////////////
//���ݴ�����ר�в���
//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color)
{
    if(len == 0)return;
    LCD_Fill(x0, y0, x0 + len - 1, y0, color);
}
//��ʵ��Բ
//x0,y0:����
//r:�뾶
//color:��ɫ
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
//������֮��ľ���ֵ
//x1,x2����ȡ��ֵ��������
//����ֵ��|x1-x2|
u16 my_abs(u16 x1, u16 x2)
{
    if(x1 > x2)return x1 - x2;
    else return x2 - x1;
}
//��һ������
//(x1,y1),(x2,y2):��������ʼ����
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    if(x1 < size || x2 < size || y1 < size || y2 < size)return;
    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if(delta_x > 0)incx = 1; //���õ�������
    else if(delta_x == 0)incx = 0; //��ֱ��
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if(delta_y > 0)incy = 1;
    else if(delta_y == 0)incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if( delta_x > delta_y)distance = delta_x; //ѡȡ��������������
    else distance = delta_y;
    for(t = 0; t <= distance + 1; t++ ) //�������
    {
        gui_fill_circle(uRow, uCol, size, color); //����
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
//���ݴ��������Ժ���
void ctp_test(void)
{
    u8 t = 0;
    u8 i = 0;
    u16 lastpos[5][2];		//���һ�ε�����
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
                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]); //����
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];

                    if(tp_dev.x[t] > (lcddev.width - 50) && tp_dev.y[t] < 50)
                    {
                        Load_Drow_Dialog();//���
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
extern u8* key_menu[15];
extern char a[10];
char b[2]={'a','b'};

//void ANJIAN(){
//	LCD_Fill(50,100,150,200,RED);
//	if(tp_dev.sta&TP_PRES_DOWN){
//	
//	if(tp_dev.x[0]<(100)&&tp_dev.x[0]>(50)&&tp_dev.y[0]<(200)&&tp_dev.y[0]>(150)){
//	display_pattern();
//	}
//	
//	}
//}
void Load_Drow_Dialog1(void)
{  
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_DrawRectangle(10, 10, 230, 230);  //������
	LCD_Fill(90,240,150,270,BLUE);        //��
	LCD_Fill(10,260,70,290,BLUE);         //��
	LCD_Fill(90,280,150,310,BLUE);        //��
	LCD_Fill(170,260,230,290,BLUE);       //��
  	POINT_COLOR=RED;//���û�����ɫ 
}
extern int right_flag;
int main(void)
{
		u8 m=0;
		u16 x=0;
		u16 y=0;
		TIM2_Configuration();
    delay_init(168);			//��ʱ��ʼ��
		bibi_init();
		UART3_Configuration();
    LCD_Init();					//LCD��ʼ��
		LED_Init();
    tp_dev.init();				//��������ʼ��
    u8 exit_keyboard=0;
    POINT_COLOR = RED;          //��������Ϊ��ɫ
    delay_ms(100);
	TIM3_Int_Init(4999,7199); //��ʼ����ʱ��


Load_keyboard(0,500,key_menu);


while (1) {
    Get_keynum(0,500);
    LCD_ShowString(200, 400, 240, 24, 24, a);
    if(right_flag==1)
		{
			break;
		}
}
LCD_Clear(WHITE);
delay_ms(100);
    Load_Drow_Dialog1();	
	snakeInit();
	GameStart();
}


