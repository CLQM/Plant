#include "lsens.h"
#include "delay.h"
#include "usart3.h"

//��ʼ������������
void Lsens_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PF8 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	T_Adc_Init();
}
//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	float tmp = 0.0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=T_Get_Adc(LSENS_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	tmp = (float)3.3 * temp_val / 4096;  //��ѹ
	printf(" dianya�� %lf \r\n", tmp);
	printf(" lux:  %d \r\n", (int)(tmp * (-20) + 205));
	
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-(temp_val/40));
}












