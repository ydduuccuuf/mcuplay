#include "touch.h"
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "24cxx.h"

_m_tp_dev tp_dev =
{
    TP_Init,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

//////////////////////////////////////////////////////////////////////////////////
//��LCD�����йصĺ���
//��һ��������
//����У׼�õ�
//x,y:����
//color:��ɫ
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color)
{
    POINT_COLOR = color;
    LCD_DrawLine(x - 12, y, x + 13, y); //����
    LCD_DrawLine(x, y - 12, x, y + 13); //����
    LCD_DrawPoint(x + 1, y + 1);
    LCD_DrawPoint(x - 1, y + 1);
    LCD_DrawPoint(x + 1, y - 1);
    LCD_DrawPoint(x - 1, y - 1);
    LCD_Draw_Circle(x, y, 6); //������Ȧ
}
//��һ�����(2*2�ĵ�)
//x,y:����
//color:��ɫ
void TP_Draw_Big_Point(u16 x, u16 y, u16 color)
{
    POINT_COLOR = color;
    LCD_DrawPoint(x, y); //���ĵ�
    LCD_DrawPoint(x + 1, y);
    LCD_DrawPoint(x, y + 1);
    LCD_DrawPoint(x + 1, y + 1);
}


//��������ʼ��
//����ֵ:0,û�н���У׼
//       1,���й�У׼
u8 TP_Init(void)
{
    if(lcddev.id == 0X5510)		//���ݴ�����
    {
        if(GT9147_Init() == 0)	//��GT9147
        {
            tp_dev.scan = GT9147_Scan;	//ɨ�躯��ָ��GT9147������ɨ��
        }

        tp_dev.touchtype |= 0X80;	//������
        tp_dev.touchtype |= lcddev.dir & 0X01; //������������
        return 0;
    }

    return 1;
}

