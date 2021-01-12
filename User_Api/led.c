#include "led.h"

//LED0--GPIOE8     PE8   
void Init_LED(void)
{ 	 
	RCC->APB2ENR|=1<<6;    //使能GPIOE时钟	   1---》  1000000
											  
	//端口配置高寄存器（GPIOE->CRH）
	//GPIOE->CRH&=~((0x0f)<<(4*0));
	GPIOE->CRH&=~(0xffffffff);
	
	//推挽输出  10MHZ
	GPIOE->CRH|=(0x11111111);       //0001
	//GPIOE->CRH|=((0x01)<<(4*0));

	//关闭led  置1（led共阳极）
  GPIOE->ODR|=0xff00;     //PE8-15输出高
	//GPIOE->ODR&=0xff00;      //PE.8输出低
	//GPIOE->ODR|=1<<8;      //PE.8输出高                 1---》  100000000
	
//	GPIOE->ODR&=~(1<<8);      //PE.8输出低
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

