#include "key_scan.h"
#include "delay.h"

/*
 * ��������Scan_Key_Configuration
 * ����  �����ö���������ʹ�õ�GPIO�˿ڼ�ģʽ
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void Scan_Key_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��ʱ��
	
	/* gpio config(PC10,PC11,PC12) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������   
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);   //���øߵ�ƽ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//ʹ��ʱ��
	/* gpio config(PE2,PE3,PE4)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������   
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOE, GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4);   //���øߵ�ƽ
}

/*
 * ��������KEY_Scan
 * ����  �����ö���������ʹ�õ�GPIO�˿ڼ�ģʽ,GPIO����Ϊ�͵�ƽ
 * ����  ������ģʽ��0->��֧��������1->֧������
 * ���  ��������ֵKEY1->1��KEY2->2��KEY3->3��KEY4->4��KEY5->5��KEY6->6
 * ����  ���ⲿ����
 * ע��  ���˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4>KEY5>KEY6!!
 */
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;	//�������ɿ���־
	
	if(mode)
	{
		key_up = 1;  	//֧������	
	}
	
	if(key_up && (KEY1 == 0||KEY2 == 0||KEY3 == 0||KEY4 == 0||KEY5 == 0||KEY6 == 0))
	{
		delay_ms(10);	 //ȥ���� 
		key_up = 0;
		
		if(KEY1 == 0)   		 return 1;//KVALUE1;
		else if(KEY2 == 0)   return 2;//KVALUE2;
		else if(KEY3 == 0)   return 3;//KVALUE3;
		else if(KEY4 == 0)   return 4;//KVALUE4;
		else if(KEY5 == 0)   return 5;//KVALUE5;
		else if(KEY6 == 0)   return 6;//KVALUE6;
		
	}else if((KEY1 == 1) && (KEY2 == 1) && (KEY3 == 1) && (KEY4 == 1) && (KEY5 == 1) && (KEY6 == 1))	
	{	
		key_up = 1;
	}
	
 	return 0;			// �ް�������
}
	


