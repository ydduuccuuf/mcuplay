#include "stm32f4xx.h"
#include "USART.h"
#include "delay.h"
unsigned char ReceiveBuffer[300] = {0};

//  结构体定义
USARTDATA   Uart3;
/**********************************************************************************************************
函数名称：UART3配置
输入参数：无
输出参数：无
函数返回：无
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

	//  开启GPIO_D的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
	//  开启串口3的时钟 
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

	/* 使能串口3 */
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
函数名称：putchar函数重定义
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
int fputc(int ch, FILE *f)
{
    USART3->SR;                                                         // 防止复位后无法打印首字符
    
    USART_SendData(USART3, (u8) ch);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {
        ; 
    }
    
    return (ch);
}

/**********************************************************************************************************
函数名称：USART3发送数据函数
输入参数：发送数据首地址和数据长度
输出参数：无
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
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4;    //LED对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //通用输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //输出推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //100MHz
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉或下拉
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);                  //初始化 GPIO
   
    I2C_Stop();
}

/**********************************************************************************************************
函数名称：配置SDA为输入
输入参数：无
输出参数：无
函数返回：无
引脚定义：
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
函数名称：配置SDA为输出
输入参数：无
输出参数：无
函数返回：无
引脚定义：
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
函数名称：I2C_delay
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_delay(void)
{
    delay_us(10);
}

/**********************************************************************************************************
函数名称：I2C_Start
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_Start(void)
{
    SDA_H;                    											//	拉高数据线
    SCL_H;                    											//	拉高时钟线
    I2C_delay();                 										//	延时
    
    SDA_L;                   	 										//	产生下降沿
    I2C_delay();                 										//	延时
    
    SCL_L;                    											//	拉低时钟线
    I2C_delay();                 										//	延时
}


/**********************************************************************************************************
函数名称：I2C_Stop
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_Stop(void)
{
    SCL_H;                    											//	拉高时钟线
    SDA_L;                    											//	拉低数据线
    I2C_delay();                 										//	延时
    
    SDA_H;                   							 				//	产生上升沿
    I2C_delay();                 										//	延时
}


/**********************************************************************************************************
函数名称：I2C_SendACK
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_SendACK(unsigned char ack)
{
    if (ack == 1)
	{
		SDA_H;
	}
    else
	{
        SDA_L;                  										//	写应答信号
	}	
    
    SCL_H;                    											//	拉高时钟线
    I2C_delay();                 										//	延时
    
    SCL_L;                    											//	拉低时钟线
    I2C_delay();                 										//	延时
}


/**********************************************************************************************************
函数名称：I2C_Start
输入参数：无
输出参数：无
函数返回：接收到的ACK
**********************************************************************************************************/
void I2C_ReceiveACK(void)
{
    unsigned char errcnt = 20;
	
    I2C_SDA_Input();
    
    I2C_delay();                 										//	延时
    
    SCL_H;                    											//	拉高时钟线
    I2C_delay();                 										//	延时
    
	while (SDA_READ())
	{
        errcnt--;
        
        if (!errcnt)
        {
            I2C_Stop();
        }
	}
    
    SCL_L;                    											//	拉低时钟线
    I2C_SDA_Output();
    I2C_delay();                 										//	延时
}


/**********************************************************************************************************
函数名称：I2C_SendByte
输入参数：发送的1字节数据
-+
函数返回：无
**********************************************************************************************************/
void I2C_SendByte(unsigned char dat)
{
    unsigned char i;
	
    for (i = 0; i < 8; i++)     										//	8位计数器
    {
        if (dat & 0x80)
		{
            SDA_H;               										//	送数据口
		}
        else
		{
            SDA_L;
		}

        dat <<= 1;              										//	移出数据的最高位
        
        SCL_H;                											//	拉高时钟线
        I2C_delay();             										//	延时
        
        SCL_L;                											//	拉低时钟线
        I2C_delay();             										//	延时
    }
    I2C_ReceiveACK();
}

/**********************************************************************************************************
函数名称：I2C_Start
输入参数：无
输出参数：无
函数返回：接收到的数据
**********************************************************************************************************/
unsigned char I2C_ReceiveByte(void)
{
    unsigned char i;
    unsigned char dat = 0;
    
	I2C_SDA_Input();
	
    for (i = 0; i < 8; i++)     										//	8位计数器
    {
        dat <<= 1;
        
        SCL_H;                											//	拉高时钟线
        I2C_delay();             										//	延时
        
        
		if (SDA_READ())
		{
			dat |= 1;             										//	读数据
		}
        
        SCL_L;                											//	拉低时钟线
        I2C_delay();             										//	延时
    }
    I2C_SDA_Output();
	
    return dat;
}
////////////
u8 Alarm_flag = 0;

/**********************************************************************************************************
函数名称：报警输入引脚配置
输入参数：无
输出参数：无
函数返回：无
//报警信号输入引脚
//ALARM(PA0)
**********************************************************************************************************/
void Alarm_GPIO_Configuration(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);               //  使能IO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              //  使能SYSCFG时钟

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                        //  输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //  设置上接
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                           //  IO口为0
    GPIO_Init(GPIOG, &GPIO_InitStructure);                             

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource5);       //  初始化中断线0

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;                          //  配置中断线为中断线0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                 //  配置中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             //  配置为下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //  配置中断线使能
    EXTI_Init(&EXTI_InitStructure);                              

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void init_CH455( void ) //初始化CH455
{
    I2C_GPIO_Configuration();
    Alarm_GPIO_Configuration();
}

void CH455_Write( u16 cmd )	//写命令
{
	I2C_Start();               //启动总线
   	I2C_SendByte(((u8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
   	I2C_SendByte((u8)cmd);               //发送数据
  	I2C_Stop();                 //结束总线 
}

u8 CH455_Read(void)		//读取按键
{
	u8 keycode;
   	I2C_Start();                //启动总线
   	I2C_SendByte((u8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);
   	keycode=I2C_ReceiveByte();      //读取数据
	I2C_Stop();                //结束总线
	return keycode;
}

