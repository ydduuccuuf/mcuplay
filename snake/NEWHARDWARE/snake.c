#include "snake.h"
#include "lcd.h"
#include "timer.h"
#include "touch.h"
#include "stdlib.h"
#include "usart.h"

Snake snake;
extern u8 count;

/*
初始化蛇
*/
void snakeInit(){
	TIM3_Int_Init(500,7199); //初始化定时器
	snake.length=3;           //初始长度为3
	
	snake.snakeX[0]=55;
	snake.snakeX[1]=55;
	snake.snakeX[2]=55;
	
	snake.snakeY[0]=55;
	snake.snakeY[1]=65;
	snake.snakeY[2]=75;
	
	snake.headX=snake.snakeX[0]; //记录下头部的位置
	snake.headY=snake.snakeY[0]; //记录下头部的位置
	snake.dir=1;  //设置运动方向
	snake.tpdir=1;//设置初始按键方向
	
	snake.life=1; //1：蛇还活着；0：蛇死亡
	
	generateFood();//生成食物
	
	refresh();    //调用函数显示出蛇和食物的位置
}
/*
每隔一秒将会执行一次，以此实现蛇的运动
*/
void snakeGo(int direct){
	u8 i;
	if(snake.life==1){
		if(abs(direct)!=abs(snake.dir)){	//如果按下的方向不是和运动的方向相同或相反
			snake.dir=direct; //将蛇运动的方向改变为按下的方向
		}else{
			direct=snake.dir;	//蛇运动的方向仍是以前运动的方向
		}
		switch(direct){
			case 1:
				snake.headY-=10;  //上：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
			case -1:
				snake.headY+=10;  //下：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
			case -2:
				snake.headX-=10;  //左：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
			case 2:
				snake.headX+=10;  //右：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
		}
		
	if(snake.headX==snake.foodX&&snake.headY==snake.foodY){ //如果吃到了食物
		snake.length++;  //长度加1
		
		for(i=1;i<snake.length;i++){ //除头部以外的坐标前移
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.foodX; //头部坐标等于食物的坐标
		snake.snakeY[0]=snake.foodY;	
		generateFood();  //再生成一个食物
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
	else{ //触碰到墙壁游戏结束
		dead();	
	}
}
/*
刷新屏幕上蛇和食物的位置
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
开始游戏
*/
void GameStart(){


	while(1){
		tp_dev.scan(0);
		if(tp_dev.sta)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				  //printf("%d %d\n\r",tp_dev.x,tp_dev.y);
				if(tp_dev.x[0]>90&&tp_dev.x[0]<150&&tp_dev.y[0]<270&&tp_dev.y[0]>240)snake.tpdir=1; //上
				if(tp_dev.x[0]>90&&tp_dev.x[0]<150&&tp_dev.y[0]<310&&tp_dev.y[0]>280)snake.tpdir=-1;//下
				if(tp_dev.x[0]>10&&tp_dev.x[0]<70&&tp_dev.y[0]<290&&tp_dev.y[0]>260)snake.tpdir=-2; //左
				if(tp_dev.x[0]>170&&tp_dev.x[0]<230&&tp_dev.y[0]<290&&tp_dev.y[0]>260)snake.tpdir=2;//右
				if(tp_dev.x[0]>90&&tp_dev.x[0]<150&&tp_dev.y[0]<180&&tp_dev.y[0]>150&&snake.life==0){
					snake.life=1;  //死了之后点击重新开始
					snakeInit();					
				}
  			   
			}
		}
		
		if(count==2){  //定时器设置的500ms的中断，1S运行一次蛇的运动函数
			count=0;			//重新计数
			snakeGo(snake.tpdir);
			refresh();	
		}
	}
}
/*
判断随机产生的食物是否处于蛇体内
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
判断是否头部是否触碰到自己
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
产生随机的食物坐标
*/
void generateFood(){
		unsigned int ran=rand()%22+1; //因为贪吃蛇运动区域为10<x<230，10<y<230，取得1到22的随机数
		snake.foodX=(ran*2+1)*5;  //随机数取奇数再乘以5
		ran=rand()%22+1;
		snake.foodY=(ran*2+1)*5;		
	while(isCover(snake.snakeX,snake.snakeY,snake.foodX,snake.foodY)){
		ran=rand()%22+1; //因为贪吃蛇运动区域为10<x<230，10<y<230，取得1到22的随机数
		snake.foodX=(ran*2+1)*5;  //随机数取奇数再乘以5
		ran=rand()%22+1;
		snake.foodY=(ran*2+1)*5;	
	}		

}

/*
在头部撞到墙之后执行死亡程序
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
	LCD_Fill(90,150,150,180,BLUE);  //点击此区域重新开始游戏
	LCD_ShowString(92,160,200,16,16,"restart"); 
	
}

