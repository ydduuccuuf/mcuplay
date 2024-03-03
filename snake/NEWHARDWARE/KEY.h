#ifndef _KEY_H_
#define _KEY_H_

/*KEY���Ŷ���
//KEY_S1      PF8
//KEY_S2      PF9
//KEY_S3      PF10
*/
#define  	KEY_S1_READ()  	GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)
#define  	KEY_S2_READ()  	GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)
#define  	KEY_S3_READ()  	GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)

void KEYGpio_Init(void);
unsigned char KeyScan(void);

//lcd
extern u8 exit_keyboard;
u8** keycontent;
u8* key_menu[15]={"1","2","3","4","5","6","7","8","9","0","DEL","Enter","Ret"," : "," None",};  //��������˵�
void Load_keyboard(u16 x,u16 y,u8 **key_content);  //���ذ�������
void Key_staset(u16 x,u16 y,u8 keyx,u8 sta);  //���ð�������״̬
u8 Get_keynum(u16 x,u16 y);  //�õ�����������

#endif
