#include "turang_exti.h"
#include "stm32f10x.h"

/*
 * ��������EXTI_Key_NVIC_Configuration
 * ����  ������Ƕ�������жϿ�����NVIC
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void EXTI_Turang_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   			//��NVIC�ж����ȼ���������Ϊ��һ��
  
  /* ����P[A|B|C|D|E]���ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;            //���õ�ΪEXTI��5~9���ж�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;            //���õ�ΪEXTI��10~15���ж�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;				//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/*
 * ��������EXTI_Key_GPIO_Configuration
 * ����  �����ö���������ʹ�õ�GPIO�˿ڼ�ģʽ
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void EXTI_Turang_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* EXTI line gpio config(PD8,PD9,PD10,PD11) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;// |GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);  //����PA8Ϊ�ⲿ�ж�8����ͨ��
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource9);  //����PD9Ϊ�ⲿ�ж�9����ͨ��
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10); //����PD10Ϊ�ⲿ�ж�10����ͨ��
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11); //����PD11Ϊ�ⲿ�ж�11����ͨ��
     
}

/*
 * ��������EXTI_Key_init
 * ����  ����ʼ����������������������ж϶˿ڼ�ģʽ
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void EXTI_Turang_init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;												

    /* config the extiline(PA) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);	//ʹ���ж����Ҫ��������ʱ��
    /* config the GPIO(PA) */
    EXTI_Turang_GPIO_Configuration();
    /* config the NVIC(PA) */
	EXTI_Turang_NVIC_Configuration();

	/* EXTI line mode config */
	/*�����ⲿ�ж�8��9��10��11���½���ʱ�����ж�*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;//|EXTI_Line9|EXTI_Line10|EXTI_Line11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�������½���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //ʹ���ж�
	EXTI_Init(&EXTI_InitStructure); 
}
