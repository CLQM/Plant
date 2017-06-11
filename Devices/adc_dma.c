#include "adc.h"
#include "stm32f10x.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

__IO uint16_t ADCConvertedValue = 0;

/**
  * @brief  ����ADC1��GPIOʱ��
  * @param  None
  * @retval None
  */
static void ADC_RCC_Configuration(void)
{
	/* ADCCLK = PCLK2/8 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	/* ʹ��DMA1ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
    /* ʹ��ADC1��GPIOB��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  ����ADC����IO��
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ����PB.0��Ϊģ������ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  ����DMA
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* DMA1 ͨ��1(channel1) ��ʼ������ -------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;    //DMA������������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;   //DMA������������ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;     //DMA����������Ϊ����Դ
	DMA_InitStructure.DMA_BufferSize = 1;    //DMA�������ݵ��ܴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //�����ַ�̶�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;        //�ڴ��ַ�̶�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   //DMA�������������ݴ�СΪ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   //�ڴ����ݴ�СΪ����
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //DMA���ȼ�Ϊ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;     //��ֹ�ڴ浽�ڴ�����ݴ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
	/* ʹ�� DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);	
}

void ADC_DMA_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	/*ADC1��GPIOʱ�ӳ�ʼ��*/
	ADC_RCC_Configuration();
	/*GPIOģʽ����*/
	GPIO_Configuration();
	/*DMAģʽ����*/
	DMA_Configuration();
	
	/* ADC1 ��ʼ������------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // ADC����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;         //��ֹʹ��ɨ��ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //ʹ������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //��ʹ���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    //�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;      //���õ�ADCͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 ͨ��������ʱ�����ã�PB.0ʹ��ADCͨ��8 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
    
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* ʹ��ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* ʹ�� ADC1 ��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* �ȴ�ADC1 ��λУ׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ���� ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ� ADC1 У׼��� */
	while(ADC_GetCalibrationStatus(ADC1));
     
	/* ���� ADC1 �������ת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
	
	
	
	
/******************* (C) ��Ȩ���� 2013 ̫ԭ�����Ƽ����޹�˾ *******************/
