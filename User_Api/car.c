#include "car.h"

//LED0--GPIOE8     PE8   
void Init_Car(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<6;    //Ê¹ÄÜGPIOEÊ±ÖÓ	   1---¡·  1000000
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
											  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void Car_Run(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_0);
	GPIO_ResetBits(GPIOD, GPIO_Pin_1);
}

void Car_Stop(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
	GPIO_ResetBits(GPIOD, GPIO_Pin_0);
}

