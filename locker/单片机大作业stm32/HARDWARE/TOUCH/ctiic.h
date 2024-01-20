#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"

//CT_SCL    PB10
//CT_SDA    PB15

//IO��������
#define CT_SDA_IN()  {GPIOB->MODER&=~(3<<(2*15));GPIOB->MODER|=0<<2*15;}	//PB15����ģʽ
#define CT_SDA_OUT() {GPIOB->MODER&=~(3<<(2*15));GPIOB->MODER|=1<<2*15;} 	//PB15���ģʽ

//IO��������
#define CT_IIC_SCL    PBout(10) 	    //SCL
#define CT_IIC_SDA    PBout(15)         //SDA	 
#define CT_READ_SDA   PBin(15)          //����SDA 


//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif







