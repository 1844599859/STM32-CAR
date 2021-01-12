#include "led.h"

//LED0--GPIOE8     PE8   
void Init_LED(void)
{ 	 
	RCC->APB2ENR|=1<<6;    //ʹ��GPIOEʱ��	   1---��  1000000
											  
	//�˿����ø߼Ĵ�����GPIOE->CRH��
	//GPIOE->CRH&=~((0x0f)<<(4*0));
	GPIOE->CRH&=~(0xffffffff);
	
	//�������  10MHZ
	GPIOE->CRH|=(0x11111111);       //0001
	//GPIOE->CRH|=((0x01)<<(4*0));

	//�ر�led  ��1��led��������
  GPIOE->ODR|=0xff00;     //PE8-15�����
	//GPIOE->ODR&=0xff00;      //PE.8�����
	//GPIOE->ODR|=1<<8;      //PE.8�����                 1---��  100000000
	
//	GPIOE->ODR&=~(1<<8);      //PE.8�����
}

void Run_Led(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);
}

void Stop_Led(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_9|GPIO_Pin_1|GPIO_Pin_13|GPIO_Pin_15);
}

void Reset_Led(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_9|GPIO_Pin_1|GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);
}

