#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "ws2812.h"
 
int8_t i;
void Delay(u32 count)
{
    u32 i = 0;
    for(; i < count; i++);
}
//½¥±ä
void red()
{
	int count=4;
	while(1)
	{
		count+=4;
		ws281x_colorWipe(ws281x_color(count, 0, 0), 1);
		if(count>=180){break;}
	}
	delay_ms(25);
	while(1)
	{
		count-=4;
		ws281x_colorWipe(ws281x_color(count, 0, 0), 1);
		if(count<=5){break;}
	}
}
void blue()
{
	int count=4;
	while(1)
	{
		count+=3;
		ws281x_colorWipe(ws281x_color(0, 0, count), 1);
		if(count>=180){break;}
	}
	delay_ms(20);
	while(1)
	{
		count-=3;
		ws281x_colorWipe(ws281x_color(0, 0, count), 1);
		if(count<=3){break;}
	}
	delay_ms(20);
}

int main(void)
{
    uart_init(115200);
    delay_init();
    ws281x_init();
	
    while(1)//½»ÌæÉÁ200ms
    {
				if(read()==1)
				{
					blue();
					red();
				}
				else
				{
					ws281x_colorWipe(ws281x_color(0, 0, 0), 20);				
				}

    }
}

