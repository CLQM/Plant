#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"

//�������˿ڶ���
#define BEEP PEout(15)	// BEEP,�������ӿ�

#define BEEPON 1
#define BEEPOFF 0

void BEEP_Init(void);	//��ʼ��
		 				    
#endif

