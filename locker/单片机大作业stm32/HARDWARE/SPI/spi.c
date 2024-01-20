#include "spi.h"
#include "stm32f4xx.h"                  // Device header

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
//void SPI1_Init(void)
//{	 
//	u16 tempreg=0;
//	RCC->AHB1ENR|=1<<0;    	//ʹ��PORTAʱ��	   
//	RCC->APB2ENR|=1<<12;   	//SPI1ʱ��ʹ�� 
//	GPIO_Set(GPIOB,7<<3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PB3~5���ù������	
//  	GPIO_AF_Set(GPIOB,3,5);	//PB3,AF5
// 	GPIO_AF_Set(GPIOB,4,5);	//PB4,AF5
// 	GPIO_AF_Set(GPIOB,5,5);	//PB5,AF5 

//	//����ֻ���SPI�ڳ�ʼ��
//	RCC->APB2RSTR|=1<<12;	//��λSPI1
//	RCC->APB2RSTR&=~(1<<12);//ֹͣ��λSPI1
//	tempreg|=0<<10;			//ȫ˫��ģʽ	
//	tempreg|=1<<9;			//���nss����
//	tempreg|=1<<8;			 
//	tempreg|=1<<2;			//SPI����  
//	tempreg|=0<<11;			//8λ���ݸ�ʽ	
//	tempreg|=1<<1;			//����ģʽ��SCKΪ1 CPOL=1 
//	tempreg|=1<<0;			//���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1  
// 	//��SPI1����APB2������.ʱ��Ƶ�����Ϊ84MhzƵ��.
//	tempreg|=7<<3;			//Fsck=Fpclk1/256
//	tempreg|=0<<7;			//MSB First  
//	tempreg|=1<<6;			//SPI���� 
//	SPI1->CR1=tempreg; 		//����CR1  
//	SPI1->I2SCFGR&=~(1<<11);//ѡ��SPIģʽ
//	SPI1_ReadWriteByte(0xff);//��������		 
//}   
////SPI1�ٶ����ú���
////SpeedSet:0~7
////SPI�ٶ�=fAPB2/2^(SpeedSet+1)
////fAPB2ʱ��һ��Ϊ84Mhz
//void SPI1_SetSpeed(u8 SpeedSet)
//{
//	SpeedSet&=0X07;			//���Ʒ�Χ
//	SPI1->CR1&=0XFFC7; 
//	SPI1->CR1|=SpeedSet<<3;	//����SPI1�ٶ�  
//	SPI1->CR1|=1<<6; 		//SPI�豸ʹ��	  
//} 
////SPI1 ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//u8 SPI1_ReadWriteByte(u8 TxData)
//{		 			 
//	while((SPI1->SR&1<<1)==0);		//�ȴ��������� 
//	SPI1->DR=TxData;	 	  		//����һ��byte  
//	while((SPI1->SR&1<<0)==0);		//�ȴ�������һ��byte  
// 	return SPI1->DR;          		//�����յ�������				    
//}
void SPI3_Init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);               //  ʹ��GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);                //  ʹ��SPI2ʱ��

    //GPIOPB3��4��5��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//  PB3��PB4��PB5���ù������	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        //  ���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      //  �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  //  100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //  ����
    GPIO_Init(GPIOB, &GPIO_InitStructure);                              //  ��ʼ��

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);            //  PB10����Ϊ SPI2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);            //  PB14����Ϊ SPI2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);            //  PB15����Ϊ SPI2

    //����ֻ���SPI�ڳ�ʼ��
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);                //  ��λSPI2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);               //  ֹͣ��λSPI2

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //  ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //  ����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //  ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //  ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //  ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //  NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//  ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //  ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //  CRCֵ����Ķ���ʽ
    SPI_Init(SPI3, &SPI_InitStructure);                                 //  ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI3, ENABLE);                                              //  ʹ��SPI����

    SPI3_ReadWriteByte(0xff);//��������		 
}   

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
unsigned char SPI3_ReadWriteByte(unsigned char TxData)
{		 			 
    //  �ȴ���������  
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
    {
    
    }
	
	SPI_I2S_SendData(SPI3, TxData);                                     //  ͨ������SPIx����һ��byte  ����
		
    //�ȴ�������һ��byte  
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
    {
    
    } 
 
	return SPI_I2S_ReceiveData(SPI3);                                   //  ����ͨ��SPIx������յ�����	
 		    
}







