#include "beep.h"
	   
//��ʼ��PB5Ϊ�����.��ʹ������ڵ�ʱ��		    
//��������ʼ��
void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��GPIOE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //BEEP-->PE.15 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 	 //�ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);	 				//���ݲ�����ʼ��GPIOB.5

	GPIO_ResetBits(GPIOE,GPIO_Pin_15);						//���0���رշ��������
}
