#include "lcd1602.h"	
#include "delay.h"	

/**
  * @brief  ����GPIOC��GPIODʱ��
  * @param  None
  * @retval None
  */
static void LCD1602_RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
}	

/**
  * @brief  ����LCD1602�����GPIO���ż�ģʽ
  * @param  None
  * @retval None
  */
static void LCD1602_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
								  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
	                              GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LCD1602_WriteCommand(u8 command)//дָ��
{
   GPIO_ResetBits(GPIOD,GPIO_Pin_6);	 //RW
   GPIO_ResetBits(GPIOD,GPIO_Pin_5);	//RS     ����ʹ��ָ��Ĵ���
   delay_ms(5); 
   GPIO_SetBits(GPIOD,GPIO_Pin_7);     //ENʹ��
   delay_ms(5);   
	 GPIOC->ODR = command;
   delay_ms(5);
   GPIO_ResetBits(GPIOD,GPIO_Pin_7);   //pin7ΪENʹ�ܶ� 
}

void LCD1602_WriteData(u8 data)
{
  GPIO_SetBits(GPIOD,GPIO_Pin_5); 	//����ʹ�����ݼĴ���
	GPIOC->ODR = data;
	GPIO_SetBits(GPIOD,GPIO_Pin_7);    //ʹ��
	delay_ms(10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //ʧ��
}

void LCD1602_Configuration(void)
{
	delay_init();//�����ط�������
	
	LCD1602_RCC_Configuration();
	
	LCD1602_GPIO_Configuration();
	
	delay_ms(400);		//��ʱ400ms
	
	LCD1602_WriteCommand(0x38);	  //��λ

	LCD1602_WriteCommand(0x0C);	 //����ʾ���رչ�꣬��겻��˸

  LCD1602_WriteCommand(0x06);   //��ʾ��ַ���ӣ���дһ�����ݺ�����һλ

	LCD1602_WriteCommand(0x01);	  //����
}

