#include "kem.h"

void KEM_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	                //ʹ��PA�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;				//PA11 PA12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			  	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 				//�����趨������ʼ��GPIOE
	GPIO_ResetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_12);						//����͵�ƽ
}