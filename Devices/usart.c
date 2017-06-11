#include "usart.h"
#include "stm32f10x.h"
#include <stdio.h>

static void USART1_RCC_Configuration(void)
{//����ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1|
	RCC_APB2Periph_AFIO,ENABLE);
}

static void USART1_GPIO_Configuration(void)
{
		//����gpio��ʼ���ṹ��
		GPIO_InitTypeDef GPIO_InitStructure;
		//��ʼ���ṹ��
		GPIO_StructInit(&GPIO_InitStructure);
		//����usart1�Ľ��ܶ˿�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA10
		//����usart1�ķ��Ͷ˿�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);// ��ʼ��pa9
}

void USART1_Configuration()
{//��ʼ������
		USART_InitTypeDef USART_InitStructure;
		//USART���ʱ�ӳ�ʼ��������
		USART1_RCC_Configuration();
		//usart���gpio��ʼ������
		USART1_GPIO_Configuration();
		
		USART_InitStructure.USART_BaudRate = 9600;//������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
		//����usart1���첽˫�����й���ģʽ
		USART_Init(USART1, &USART_InitStructure);
		//ʹ��usart1�Ľ����ж�
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		//�ر�usart1�ķ����ж�
		USART_ITConfig(USART1,USART_IT_TXE, ENABLE);
		//ʹ��usart1
		USART_Cmd(USART1,ENABLE);
}
#if 1
/**
  * @brief  �ض���C�����е�printf������USART
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8)ch);

  /* ѭ��ֱ�����ͽ��� */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}
#endif
