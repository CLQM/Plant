#include "lcd12864.h"
#include "delay.h"
#include <stdio.h>

void LCD12864_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*��12864�������źŽ�������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*��12864�������źŽ�������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void LCD12864_busy(void)
{
	unsigned int timeout = 10;
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);               //EN ʧ�� 
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);               //RS=L
	GPIO_SetBits(GPIOB,GPIO_Pin_13);                 //RW=H
	GPIO_SetBits(GPIOB,GPIO_Pin_14);                 //EN ʹ��=H 
	while((GPIOD->ODR & 0x80) && --timeout != 0);    //æ״̬���
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);               //EN ʧ��
	
//     uint16_t flag = 0x00;
// 	  GPIO_ResetBits(GPIOB,GPIO_Pin_12);    //RS=L 
// 	  GPIO_SetBits(GPIOB,GPIO_Pin_13);     //RW ���� 
// 	  do
// 		{
// 			  GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN ʧ�� 
// 			  Delay_us(5);
// 			  GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN ʹ��=H 
// 			  Delay_us(5);
// 			  flag = (uint16_t)LCD_BF;
//     }while(flag);
// 		GPIO_ResetBits(GPIOB,GPIO_Pin_14);               //EN ʧ��
}

void LCD12864_WriteCommand(u8 command) 
{ 
//	LCD12864_busy();
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);    //RW=L   
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);    //RS=L 
	delay_us(15);
  GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN ʹ��=H 
//	DATAUOT |= command;
  delay_us(15);	
  GPIOD->ODR = (( GPIOD->ODR & 0xff00 )| command ) ;  
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN ʧ�� 
} 
 
void LCD12864_WriteData(u8 data) 
{ 
//	LCD12864_busy();
	GPIO_SetBits(GPIOB,GPIO_Pin_12);    //RS ���� 
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);    //RW ���� 
	delay_us(15);
  GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN ʹ�� 
  GPIOD->ODR = (( GPIOD->ODR & 0xff00 )| data ) ; 
//	DATAUOT |= data;
  delay_us(15);
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN ʧ��
} 

u8 LCD12864_ReadData(void)
{
	u8 temp;
//	LCD12864_busy();
	GPIOD->ODR = (( GPIOD->ODR & 0xff00 )| 0x00ff ) ;
	GPIO_SetBits(GPIOB,GPIO_Pin_12);      //RS ����
	GPIO_SetBits(GPIOB,GPIO_Pin_13);      //RW ���� 
	GPIO_SetBits(GPIOB,GPIO_Pin_14);      //EN ʹ�� 
	delay_us(15);
	temp = GPIOD->ODR;
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN ʧ��
	return temp;
}

void LCD12864_ShowStr(u8 *s)    //��ʾ�ַ����ĺ��� 
{ 
     while(*s != '\0') 
     { 
         LCD12864_WriteData(*s); 
         s++; 
         delay_ms(2); 
     } 
} 
 
void LCD12864_ShowStr_Line(u8 *s,u8 line)    //�ڵڼ�����ʾ�ַ����ĺ���
{ 
     switch(line) 
    { 
      case 1: 
        LCD12864_WriteCommand(0x80); 
        LCD12864_ShowStr(s); 
         break; 
      case 2: 
        LCD12864_WriteCommand(0x90); 
			  LCD12864_ShowStr(s); 
         break; 
      case 3: 
        LCD12864_WriteCommand(0x88); 
        LCD12864_ShowStr(s); 
         break; 
      case 4: 
        LCD12864_WriteCommand(0x98); 
        LCD12864_ShowStr(s); 
         break; 
    } 
} 

void LCD12864_PutString(u8 x,u8 y,u8 *s)          //��X��Y����ʾ�ַ����ĺ���
{
	switch(y)
	{
		case 1:
			LCD12864_WriteCommand(0x80+x);
		  break;
		case 2:
			LCD12864_WriteCommand(0x90+x);
		  break;
		case 3:
			LCD12864_WriteCommand(0x88+x);
		  break;
		case 4:
			LCD12864_WriteCommand(0x98+x);
		  break;
		default:
			break;
  }
	
	while(*s != '\0')
	{
		LCD12864_WriteData(*s); 
    s++; 
    delay_ms(2);
  }
}
	

/*�����ǹ�궨λ��������һ������Ϊ�����꣬�ڶ���Ϊ�����꣬��ʼ������1��1�С�
ֻ����16����Ŀ��Ϊ��λ�ƶ���*/

void CursorLocate(int row, int col) 
{
	switch(row)
	{
		case 1: LCD12864_WriteCommand(0x80+col-1); break;
		case 2: LCD12864_WriteCommand(0x90+col-1); break;
		case 3: LCD12864_WriteCommand(0x88+col-1); break;
		case 4: LCD12864_WriteCommand(0x98+col-1); break;
	}
}

void drawdot_12864(u8 y,u8 x,u8 type)
{
	u8 X,Y,k;
	u8 DH,DL;
	if(y >= 0 && y <= 63 && x >= 0 && x <= 127)
	{
		if(y < 32)
		{
			X = 0x80 + (x >> 4);
			Y = 0x80 + y;
    }else
		{
			X = 0x88 + (x >> 4);
			Y = 0x80 + (y - 32);
    }
		LCD12864_WriteCommand(0x34);
		LCD12864_WriteCommand(Y);
		LCD12864_WriteCommand(X);
		DH = LCD12864_ReadData();
		DH = LCD12864_ReadData();
		DL = LCD12864_ReadData();
		
		k = x % 16;
		
		switch(type)
		{
			case 0: 
				if(k < 8)
				{
					DH &= ~(0x01 << (7 - k));
        }else
				{
          DL &= ~(0x01 << (7 - (k%8)));
				}
				break;
			case 1:
					if(k < 8)
					{
						DH |= (0x01 << (7 - k));
          }else
					{
						DL |= (0x01 << (7 - (k%8)));
          }
					break;
				default: break;	
    }
		LCD12864_WriteCommand(Y);
		LCD12864_WriteCommand(X);
		LCD12864_WriteData(DH);
		LCD12864_WriteData(DL);
		LCD12864_WriteCommand(0x30);
  }
}

void LCD12864_clear(void)
{
	u8 i,j;
	LCD12864_WriteCommand(0x34);
	for(i = 0;i < 32;i++)
	{
		LCD12864_WriteCommand(0x80 + i);
		LCD12864_WriteCommand(0x80);
		for(j = 0;j < 32;j++)
		{
			LCD12864_WriteData(0x00);
    }
  }
	LCD12864_WriteCommand(0x36);
}

void LCD12864_bar(u8 left,u8 up,u8 right,u8 down,u8 color)
{
	u8 temp;
	LCD12864_WriteCommand(0x34);             //����չָ��
	if(left > right)
	{
		temp = left;
		left = right;
		right = temp;
  }
	if(up > down)
	{
		temp = up;
		up = down;
		down = temp;
  }
	
	for(;up <= down;up++)
	{
	  for(temp = left;temp <= right;temp++)
		{
			drawdot_12864(up,temp,color);
    }
	}
	//LCD12864_WriteCommand(0x36);
}





