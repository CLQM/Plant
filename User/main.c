#include "led.h"
#include "sys.h"
#include <stdio.h>
#include "turang_exti.h"
#include "usart.h"
#include "usart3.h"
#include "delay.h"
// #include "guangmin_adc.h"
//#include "digital_tube.h"
//#include "adc.h"
#include "lsens.h"
#include "lcd12864.h"
#include "kem.h"
#include "beep.h"
#include "adc.h"
#include "key_scan.h"
#include "timer3.h"

#define LSENS_READ_TIMES	10		//����������������Ĵ���

int lcd12864_up   = 16;           //lcd12864����������Ӱ�Ͻ�
int lcd12864_down = 31;           //lcd12864����������Ӱ�½�

int time_num[5] = {14400, 10800, 7200, 3600, 600};	//4Сʱ 3Сʱ 2Сʱ 1Сʱ 1����(Debug)

void itoa1(u8 * a,int x);
extern __IO uint16_t ADCConvertedValue;

void Cursor_move()
{
	LCD12864_clear();
  LCD12864_bar(0, lcd12864_up, 127, lcd12864_down, 1);   
  LCD12864_WriteCommand(0x36);    //���û�ͼ��ʾ��
}

void Beep_ming(void)
{
	int i = 0;
	
	BEEP = BEEPON;
	for(i = 655350; i > 0; i--);
	for(i = 655350; i > 0; i--);
	for(i = 655350; i > 0; i--);
	BEEP = BEEPOFF;	
}

void Show_wendu_guangzhao(void)
{
	LCD12864_WriteCommand(0x01);	                          //����
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
	LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
	LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);	
}

void Show_success(void)
{
	LCD12864_WriteCommand(0x01);	                          //����
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
	LCD12864_ShowStr_Line("�����ɹ�        ",1);
}

int main(void)
{
	int i = 0;
	float value = 0.0;
	u16 tmp;
	int ivalue;
	char buf[1];
	u32 temp_val=0;
	u8 t;
	u8 adcx = 0;
	u8 key_value = 0;
	int temp_index = 0;
	int time_index = 0;
	int time_counter = 0;
	int time_num_index = 4;	//��һ���ӿ�ʼ

//	Lsens_Init();		//��ʼ���������贫����
	LED_Init();			//led�Ƴ�ʼ�� E7
	KEM_Init();			//�̵�����ʼ��
	BEEP_Init();		//������
	delay_init();		//�ӳٺ�����ʼ��
	EXTI_Turang_init();					//����ʪ���жϳ�ʼ��  D8
	LCD12864_init();            //LCD12864��ʼ��
//		ADC_DMA_Configuration();			//ʹ��DMAģʽ��ADC�������ĳ�ʼ��
	USART1_Configuration();
	usart3_init(9600);
	Scan_Key_Configuration();		//����������ʼ��

	Lsens_Init(); 			//????????
	
	delay_ms(1000);
	LCD12864_WriteCommand(0x30);                            //�򿪻���ָ�
	LCD12864_WriteCommand(0x0c);                            //�ع��
	LCD12864_WriteCommand(0x01);	                          //����
 	LCD12864_WriteCommand(0x06);                            //����ģʽ����

	//��������
	LCD12864_WriteCommand(0x01);	                          //����
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
  LCD12864_ShowStr_Line("  ̫ԭʦ��ѧԺ  ",1);
  LCD12864_ShowStr_Line("    �����ϵ    ",2);
 	LCD12864_ShowStr_Line("  ̫ԭʦ��ѧԺ  ",3);
 	LCD12864_ShowStr_Line("  ��ƣ���ǿ��  ",4);
	delay_ms(8000);
	//��������
	
	LCD12864_WriteCommand(0x01);	                          //����
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
	LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
	LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);	

	while(1)
	{
		delay_ms(10);
		
		if(time_counter > time_num[time_num_index])
		{//1Ϊ100ms 10 Ϊ1s 600Ϊ 1 3600 Ϊ 1h
			KEM2 = KEMOFF;
			time_counter = 0;		//���¼���
			time_index = 0;			//�رռ���
		}
		
		key_value = 0;  //δ��ʼ��ʱ��Σ��
		key_value = KEY_Scan(0);	//�õ���ֵ

		if(key_value > 0)
		{
			Beep_ming();
			
			if(key_value == 1)
			{//OK
				LCD12864_WriteCommand(0x01);	                          //����
				delay_ms(1);
				LCD12864_WriteCommand(0x30);
				LCD12864_ShowStr_Line("1:����ˮ��      ", 1);
				LCD12864_ShowStr_Line("2:���������    ", 2);
				LCD12864_ShowStr_Line("3:�ر�ˮ��      ", 3);
				LCD12864_ShowStr_Line("4:�رղ����    ", 4);
				
				i = 1;
				while(i)
				{
					delay_ms(100);
					
					if(time_index)
					{
						time_counter++;	//��һ�ε�����
					}
					
					key_value = 0;  //δ��ʼ��ʱ��Σ��
					key_value = KEY_Scan(0);	//���µõ���ֵ
					if(key_value > 0)
					{
						Beep_ming();
					}
					if(key_value == 1)
					{//����ˮ��
						KEM1 = KEMON;
						
						LCD12864_WriteCommand(0x01);	                          //����
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("�����ɹ�        ",1);
						
						delay_ms(600);			//���ۿ��ü�
						
						LCD12864_WriteCommand(0x01);	                          //����
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
						LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);	
						
						i = 0;		//�˳�ѭ��
					}
					else if(key_value == 2)
					{//���������
						LCD12864_WriteCommand(0x01);	                          //����
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("1:ľ�� 4Сʱ    ", 1);
						LCD12864_ShowStr_Line("2:�ݱ� 3Сʱ    ", 2);
						LCD12864_ShowStr_Line("3:�ٱ� 2Сʱ    ", 3);
						LCD12864_ShowStr_Line("4:���� 1Сʱ    ", 4);	
						
						i = 1;		//����һ����ѭ��
						while(i)
						{
							delay_ms(100);
					
							if(time_index)
							{
								time_counter++;	//��һ�ε�����
							}
							
							key_value = 0;  //δ��ʼ��ʱ��Σ��
							key_value = KEY_Scan(0);	//���µõ���ֵ
							if(key_value > 0)
							{
								Beep_ming();	//�������·�������
							}
							
							if(key_value == 1)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//���ó�����ļ�ʱ��
								time_num_index = 0;		//ѡ�����е�ʱ��4Сʱ
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("�����ɹ�        ",1);
								LCD12864_ShowStr_Line("�����Զ��رջ���",2);
								LCD12864_ShowStr_Line("   4Сʱ        ",3);
								
								delay_ms(600);			//���ۿ��ü�
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
								LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
								
								i = 0;		//�˳�ѡ��ֲ���ѭ��								
							}
							else if(key_value == 2)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//���ó�����ļ�ʱ��
								time_num_index = 1;
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("�����ɹ�        ",1);
								LCD12864_ShowStr_Line("�����Զ��رջ���",2);
								LCD12864_ShowStr_Line("   3Сʱ        ",3);
								
								delay_ms(600);			//���ۿ��ü�
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
								LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
								
								i = 0;		//�˳�ѡ��ֲ���ѭ��	
							}
							else if(key_value == 3)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//���ó�����ļ�ʱ��
								time_num_index = 2;
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("�����ɹ�        ",1);
								LCD12864_ShowStr_Line("�����Զ��رջ���",2);
								LCD12864_ShowStr_Line("   2Сʱ        ",3);
								
								delay_ms(600);			//���ۿ��ü�
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
								LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
								
								i = 0;		//�˳�ѡ��ֲ���ѭ��	
							}
							else if(key_value == 4)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//���ó�����ļ�ʱ��
								time_num_index = 3;
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("�����ɹ�        ",1);
								LCD12864_ShowStr_Line("�����Զ��رջ���",2);
								LCD12864_ShowStr_Line("   1Сʱ        ",3);
								
								delay_ms(600);			//���ۿ��ü�
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
								LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
								
								i = 0;		//�˳�ѡ��ֲ���ѭ��	
							}
							else if(key_value == 5)
							{//debug
								KEM2 = KEMON;
								
								time_index = 1;		//���ó�����ļ�ʱ��
								time_num_index = 4;
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("�����ɹ�        ",1);
								LCD12864_ShowStr_Line("�����Զ��رջ���",2);
								LCD12864_ShowStr_Line("   1����        ",3);
								
								delay_ms(600);			//���ۿ��ü�
								
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
								LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
								
								i = 0;		//�˳�ѡ��ֲ���ѭ��	
							}
							else if(key_value == 6)
							{
								LCD12864_WriteCommand(0x01);	                          //����
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
								LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);	
								
								i = 0;		//�˳�ѭ��
							}
						}
// 						KEM2 = KEMON;
// //						TIM3_Int_Init(4999,7199);//10Khz����Ƶ�ʣ�������5000Ϊ500ms
// 						
// 						time_index = 1;		//���ó�����ļ�ʱ��
// 						
// 						LCD12864_WriteCommand(0x01);	                          //����
// 						delay_ms(1);
// 						LCD12864_WriteCommand(0x30);
// 						LCD12864_ShowStr_Line("�����ɹ�        ",1);
// 						LCD12864_ShowStr_Line("�����Զ��رջ���",2);
// 						LCD12864_ShowStr_Line("   2Сʱ        ",3);
// 						
// 						delay_ms(600);			//���ۿ��ü�
// 						
// 						LCD12864_WriteCommand(0x01);	                          //����
// 						delay_ms(1);
// 						LCD12864_WriteCommand(0x30);
// 						LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
// 						LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);	
						
						i = 0;		//�˳���������Ƶ�ѭ��
					}
					else if(key_value == 3)
					{
						if(KEM1 == KEMON)
						{
							KEM1 = KEMOFF;
							
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //����
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  ˮ���Ѿ��ر�  ",1);
							
							delay_ms(600);			//���ۿ��ü�
						
							LCD12864_WriteCommand(0x01);	                          //����
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
							LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
							
							i = 0;		//�˳�ѭ��
						}
						else
						{
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //����
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  ˮ�ùر�ʧ��  ",1);
						}
					}
					else if(key_value == 4)
					{
						if(KEM2 == KEMON)
						{
							KEM2 = KEMOFF;
							
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //����
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  ������ѹر�  ",1);
							
							delay_ms(600);			//���ۿ��ü�
						
							LCD12864_WriteCommand(0x01);	                          //����
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
							LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);
							
							time_counter = 0;		//���¼���
							time_index = 0;			//�رռ���
							
							i = 0;		//�˳�ѭ��
						}
						else
						{
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //����
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  ����ƹر�ʧ��",1);
						}
					}
					else if(key_value == 6)
					{
						LCD12864_WriteCommand(0x01);	                          //����
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("  ����ǿ�ȣ�    ",1);
						LCD12864_ShowStr_Line("  ����ʪ�ȣ�    ",3);	
						
						i = 0;		//�˳�ѭ��
					}
				}
			}
		}

		delay_ms(100);			//Ӧ����һ���ȽϺ����ʱ��
		if(time_index)
		{
			time_counter++;	//��һ�ε�����
		}
		temp_index++;				//0~9
		
//		value = (float)3.3 * ADCConvertedValue / 4096;  //��ѹ

//		adcx = string[0] * (-20) + 205;	//��ʽ
		if(temp_index == 9)
		{
			temp_index = 0;
			adcx = Lsens_Get_Val();
	// 		value = value * (-20) + 205;
	// 		i = (int)value;
	// 		adcx = i;
	//		itoa1(string, i);
			i = adcx;
	//		printf("  value: %d\r\n", i);
			
			sprintf(buf, "%d", i);		//����⵽��ֵ������ʾ������
			LCD12864_WriteCommand(0x30);
			LCD12864_ShowStr_Line(buf, 2);   //0~99
//			LCD12864_ShowStr_Line("buf             ", 2);
			
			if(adcx < 20 && adcx > 0)
			{//!0
				LED2 = LEDON;
				LED3 = LEDOFF;
				LED4 = LEDOFF;
			}
			else if(adcx < 50)
			{
				LED2 = LEDOFF;
				LED3 = LEDON;
				LED4 = LEDOFF;
			}
			else if(adcx < 100)
			{
				LED2 = LEDOFF;
				LED3 = LEDOFF;
				LED4 = LEDON;
			}
			else
			{
				LED2 = LEDOFF;
				LED3 = LEDOFF;
				LED4 = LEDOFF;
			}
//			string[3] = '\0';
// 		for( i = 0; i < 3; i++)
// 			LCD12864_PutString(2 + i, 2  ,  string +i );

// 		for(t=0;t<LSENS_READ_TIMES;t++)
// 		{
// 			temp_val += ADCConvertedValue;	//??ADC?
// 			delay_ms(5);
// 		}
// 		temp_val/=LSENS_READ_TIMES;//????? 
// 		if(temp_val>4000)temp_val=4000;
// 		string[0]	= (u8)(100-(temp_val/40));
// 		if(string[0])
// 		{
// 			LED2 = LEDON;
// 		}
// 		string[1] = '\0';
// 		
// 		LCD12864_PutString(2, 2 ,  string );
// 		//��ʾADCת���������Чλ��3λ��С��λ2λ
// //		value *= 100; //��ֵ����10000������ʾ���ݱ���С����2λ
// 		ivalue = (int)value;
// 		itoa1(string, ivalue);
// 		
// 		//״̬��ʾ
// //		LCD12864_WriteCommand(0x01);	                          //����
// 		
// 		if(ivalue >= 1000)
// 		{
// //			KEM1 = KEMOFF;
// 		}
// 		else
// 		{
// //			KEM1 = KEMON;
// 		}
			delay_ms(100);			//Ӧ����һ���ȽϺ����ʱ��
			if(time_index)
			{
				time_counter++;	//��һ�ε�����
			}
		}
	}
}
	void itoa1(u8 * a,int x)
{
	int i,j = 0;
	int tmp;
	int shuzu[10];

	for(i = 0; x > 0; i ++)
	{

			tmp = x % 10;
			shuzu[i] = tmp;
			x = x / 10;
	}

	for(i = i - 1; i >= 0 ; i--,j++)
	{
			a[j] = shuzu[i] + 48;
	}
	a[++j] = '\0';
}
