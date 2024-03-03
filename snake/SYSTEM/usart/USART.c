#include "stm32f4xx.h"
#include "USART.h"
#include "delay.h"
unsigned char ReceiveBuffer[300] = {0};

//  �ṹ�嶨��
USARTDATA   Uart3;
/**********************************************************************************************************
�������ƣ�UART3����
�����������
�����������
�������أ���
**********************************************************************************************************/
// USART3_TX	 PC10
// USART3_RX	 PC11
void UART3_Configuration(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;
    
    Uart3.ReceiveFinish = 0;
    Uart3.RXlenth = 0;
    Uart3.Time = 0;
    Uart3.Rxbuf = ReceiveBuffer;

	//  ����GPIO_D��ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
	//  ��������3��ʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);


	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART3, &USART_InitStructure);

	/* ʹ�ܴ���3 */
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    /* NVIC configuration */
    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable USART */
    USART_Cmd(USART3, ENABLE);
}

/**********************************************************************************************************
�������ƣ�putchar�����ض���
�����������
�����������
�������أ���
**********************************************************************************************************/
int fputc(int ch, FILE *f)
{
    USART3->SR;                                                         // ��ֹ��λ���޷���ӡ���ַ�
    
    USART_SendData(USART3, (u8) ch);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {
        ; 
    }
    
    return (ch);
}

/**********************************************************************************************************
�������ƣ�USART3�������ݺ���
������������������׵�ַ�����ݳ���
�����������
**********************************************************************************************************/
void USART3_Senddata(unsigned char *Data, unsigned int length)
{
    while(length--)
    {
        USART_SendData(USART3,*Data++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
    }
}
//
void I2C_GPIO_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4;    //LED��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //ͨ�����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //100MHz
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //������������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);                  //��ʼ�� GPIO
   
    I2C_Stop();
}

/**********************************************************************************************************
�������ƣ�����SDAΪ����
�����������
�����������
�������أ���
���Ŷ��壺
//SDA	PF0
**********************************************************************************************************/
void I2C_SDA_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**********************************************************************************************************
�������ƣ�����SDAΪ���
�����������
�����������
�������أ���
���Ŷ��壺
//SDA	PF0
**********************************************************************************************************/
void I2C_SDA_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**********************************************************************************************************
�������ƣ�I2C_delay
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_delay(void)
{
    delay_us(10);
}

/**********************************************************************************************************
�������ƣ�I2C_Start
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_Start(void)
{
    SDA_H;                    											//	����������
    SCL_H;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
    
    SDA_L;                   	 										//	�����½���
    I2C_delay();                 										//	��ʱ
    
    SCL_L;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
}


/**********************************************************************************************************
�������ƣ�I2C_Stop
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_Stop(void)
{
    SCL_H;                    											//	����ʱ����
    SDA_L;                    											//	����������
    I2C_delay();                 										//	��ʱ
    
    SDA_H;                   							 				//	����������
    I2C_delay();                 										//	��ʱ
}


/**********************************************************************************************************
�������ƣ�I2C_SendACK
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_SendACK(unsigned char ack)
{
    if (ack == 1)
	{
		SDA_H;
	}
    else
	{
        SDA_L;                  										//	дӦ���ź�
	}	
    
    SCL_H;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
    
    SCL_L;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
}


/**********************************************************************************************************
�������ƣ�I2C_Start
�����������
�����������
�������أ����յ���ACK
**********************************************************************************************************/
void I2C_ReceiveACK(void)
{
    unsigned char errcnt = 20;
	
    I2C_SDA_Input();
    
    I2C_delay();                 										//	��ʱ
    
    SCL_H;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
    
	while (SDA_READ())
	{
        errcnt--;
        
        if (!errcnt)
        {
            I2C_Stop();
        }
	}
    
    SCL_L;                    											//	����ʱ����
    I2C_SDA_Output();
    I2C_delay();                 										//	��ʱ
}


/**********************************************************************************************************
�������ƣ�I2C_SendByte
������������͵�1�ֽ�����
-+
�������أ���
**********************************************************************************************************/
void I2C_SendByte(unsigned char dat)
{
    unsigned char i;
	
    for (i = 0; i < 8; i++)     										//	8λ������
    {
        if (dat & 0x80)
		{
            SDA_H;               										//	�����ݿ�
		}
        else
		{
            SDA_L;
		}

        dat <<= 1;              										//	�Ƴ����ݵ����λ
        
        SCL_H;                											//	����ʱ����
        I2C_delay();             										//	��ʱ
        
        SCL_L;                											//	����ʱ����
        I2C_delay();             										//	��ʱ
    }
    I2C_ReceiveACK();
}

/**********************************************************************************************************
�������ƣ�I2C_Start
�����������
�����������
�������أ����յ�������
**********************************************************************************************************/
unsigned char I2C_ReceiveByte(void)
{
    unsigned char i;
    unsigned char dat = 0;
    
	I2C_SDA_Input();
	
    for (i = 0; i < 8; i++)     										//	8λ������
    {
        dat <<= 1;
        
        SCL_H;                											//	����ʱ����
        I2C_delay();             										//	��ʱ
        
        
		if (SDA_READ())
		{
			dat |= 1;             										//	������
		}
        
        SCL_L;                											//	����ʱ����
        I2C_delay();             										//	��ʱ
    }
    I2C_SDA_Output();
	
    return dat;
}
////////////
u8 Alarm_flag = 0;

/**********************************************************************************************************
�������ƣ�����������������
�����������
�����������
�������أ���
//�����ź���������
//ALARM(PA0)
**********************************************************************************************************/
void Alarm_GPIO_Configuration(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);               //  ʹ��IOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              //  ʹ��SYSCFGʱ��

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                        //  ����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //  �����Ͻ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                           //  IO��Ϊ0
    GPIO_Init(GPIOG, &GPIO_InitStructure);                             

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource5);       //  ��ʼ���ж���0

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;                          //  �����ж���Ϊ�ж���0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                 //  �����ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             //  ����Ϊ�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //  �����ж���ʹ��
    EXTI_Init(&EXTI_InitStructure);                              

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void init_CH455( void ) //��ʼ��CH455
{
    I2C_GPIO_Configuration();
    Alarm_GPIO_Configuration();
}

void CH455_Write( u16 cmd )	//д����
{
	I2C_Start();               //��������
   	I2C_SendByte(((u8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
   	I2C_SendByte((u8)cmd);               //��������
  	I2C_Stop();                 //�������� 
}

u8 CH455_Read(void)		//��ȡ����
{
	u8 keycode;
   	I2C_Start();                //��������
   	I2C_SendByte((u8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);
   	keycode=I2C_ReceiveByte();      //��ȡ����
	I2C_Stop();                //��������
	return keycode;
}

