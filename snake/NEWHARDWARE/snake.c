#include "snake.h"
#include "lcd.h"
#include "timer.h"
#include "touch.h"
#include "stdlib.h"
#include "usart.h"

Snake snake;
extern u8 count;

/*
��ʼ����
*/
void snakeInit(){
	TIM3_Int_Init(500,7199); //��ʼ����ʱ��
	snake.length=3;           //��ʼ����Ϊ3
	
	snake.snakeX[0]=55;
	snake.snakeX[1]=55;
	snake.snakeX[2]=55;
	
	snake.snakeY[0]=55;
	snake.snakeY[1]=65;
	snake.snakeY[2]=75;
	
	snake.headX=snake.snakeX[0]; //��¼��ͷ����λ��
	snake.headY=snake.snakeY[0]; //��¼��ͷ����λ��
	snake.dir=1;  //�����˶�����
	snake.tpdir=1;//���ó�ʼ��������
	
	snake.life=1; //1���߻����ţ�0��������
	
	generateFood();//����ʳ��
	
	refresh();    //���ú�����ʾ���ߺ�ʳ���λ��
}
/*
ÿ��һ�뽫��ִ��һ�Σ��Դ�ʵ���ߵ��˶�
*/
void snakeGo(int direct){
	u8 i;
	if(snake.life==1){
		if(abs(direct)!=abs(snake.dir)){	//������µķ����Ǻ��˶��ķ�����ͬ���෴
			snake.dir=direct; //�����˶��ķ���ı�Ϊ���µķ���
		}else{
			direct=snake.dir;	//���˶��ķ���������ǰ�˶��ķ���
		}
		switch(direct){
			case 1:
				snake.headY-=10;  //�ϣ��ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
			case -1:
				snake.headY+=10;  //�£��ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
			case -2:
				snake.headX-=10;  //���ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
			case 2:
				snake.headX+=10;  //�ң��ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
		}
		
	if(snake.headX==snake.foodX&&snake.headY==snake.foodY){ //����Ե���ʳ��
		snake.length++;  //���ȼ�1
		
		for(i=1;i<snake.length;i++){ //��ͷ�����������ǰ��
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.foodX; //ͷ���������ʳ�������
		snake.snakeY[0]=snake.foodY;	
		generateFood();  //������һ��ʳ��
	}else{
		for(i=1;i<snake.length;i++){
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.headX;
		snake.snakeY[0]=snake.headY;			
	}	
	}
	if(snake.headY<230&&snake.headY>10&&snake.headX<230&&snake.headX>10&&isNearSelf()){
		snake.life=1;
	}
	else{ //������ǽ����Ϸ����
		dead();	
	}
}
/*
ˢ����Ļ���ߺ�ʳ���λ��
*/

void refresh(){
	if(snake.life==1){
	u8 i;
	LCD_Fill(11,11,229,229,WHITE);
	for(i=0;i<snake.length;i++){
		Draw_Circle_Solid(snake.snakeX[i],snake.snakeY[i],5);
	}
	Draw_Circle_Solid(snake.foodX,snake.foodY,5);
}
}
/*
��ʼ��Ϸ
*/
void GameStart(){


	while(1){
		tp_dev.scan(0);
		if(tp_dev.sta)			//������������
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				  //printf("%d %d\n\r",tp_dev.x,tp_dev.y);
				if(tp_dev.x[0]>90&&tp_dev.x[0]<150&&tp_dev.y[0]<270&&tp_dev.y[0]>240)snake.tpdir=1; //��
				if(tp_dev.x[0]>90&&tp_dev.x[0]<150&&tp_dev.y[0]<310&&tp_dev.y[0]>280)snake.tpdir=-1;//��
				if(tp_dev.x[0]>10&&tp_dev.x[0]<70&&tp_dev.y[0]<290&&tp_dev.y[0]>260)snake.tpdir=-2; //��
				if(tp_dev.x[0]>170&&tp_dev.x[0]<230&&tp_dev.y[0]<290&&tp_dev.y[0]>260)snake.tpdir=2;//��
				if(tp_dev.x[0]>90&&tp_dev.x[0]<150&&tp_dev.y[0]<180&&tp_dev.y[0]>150&&snake.life==0){
					snake.life=1;  //����֮�������¿�ʼ
					snakeInit();					
				}
  			   
			}
		}
		
		if(count==2){  //��ʱ�����õ�500ms���жϣ�1S����һ���ߵ��˶�����
			count=0;			//���¼���
			snakeGo(snake.tpdir);
			refresh();	
		}
	}
}
/*
�ж����������ʳ���Ƿ���������
*/
u8 isCover(u8 snakeX[],u8 snakeY[],u8 foodX,u8 foodY){
	u8 i;
	for(i=0;i<snake.length;i++){
		if(snakeX[i]==foodX&&snakeY[i]==foodY)
		{
		return 1;
		}
		
	}
	return 0;
}

/*
�ж��Ƿ�ͷ���Ƿ������Լ�
*/
u8 isNearSelf(){
	u8 i;
	for(i=1;i<snake.length;i++){
		if(snake.snakeX[i]==snake.headX&&snake.snakeY[i]==snake.headY)
		{
		return 0;
		}	
	}
	return 1;
}
/*
���������ʳ������
*/
void generateFood(){
		unsigned int ran=rand()%22+1; //��Ϊ̰�����˶�����Ϊ10<x<230��10<y<230��ȡ��1��22�������
		snake.foodX=(ran*2+1)*5;  //�����ȡ�����ٳ���5
		ran=rand()%22+1;
		snake.foodY=(ran*2+1)*5;		
	while(isCover(snake.snakeX,snake.snakeY,snake.foodX,snake.foodY)){
		ran=rand()%22+1; //��Ϊ̰�����˶�����Ϊ10<x<230��10<y<230��ȡ��1��22�������
		snake.foodX=(ran*2+1)*5;  //�����ȡ�����ٳ���5
		ran=rand()%22+1;
		snake.foodY=(ran*2+1)*5;	
	}		

}

/*
��ͷ��ײ��ǽ֮��ִ����������
*/
void dead(){
	snake.length=0;
	
	snake.snakeX[0]=0;
	snake.snakeX[1]=0;
	snake.snakeX[2]=0;
	
	snake.snakeY[0]=0;
	snake.snakeY[1]=0;
	snake.snakeY[2]=0;
	
	snake.headX=snake.snakeX[0];
	snake.headY=snake.snakeY[0];
	snake.dir=0;  
	
	snake.life=0;	
	LCD_ShowString(80,50,200,16,16,"You dead!!!");
	LCD_Fill(90,150,150,180,BLUE);  //������������¿�ʼ��Ϸ
	LCD_ShowString(92,160,200,16,16,"restart"); 
	
}
