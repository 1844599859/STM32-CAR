#include "cry.h"

static void delay(int ms)
{
	for(int i=0;i<ms;i++)
		for(int j=0;j<1000;j++);
}

void Init_Cry(void)
{
	// 结构体初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	// 时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	// 设置结构体
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	// 设置端口,PE0 推挽输出 10MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	// 关闭蜂鸣器
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
}

void Cry(int ms)
{
	// 开启蜂鸣器
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	// 等待时延
	delay(ms);
	// 关闭蜂鸣器
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
}
