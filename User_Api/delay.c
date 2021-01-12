#include "delay.h"
uint32_t  systime=0;

void Init_Delay(void)
{
	SysTick_Config(72000-1);//系统滴答定时器就初始化完成了
}

void Delay_1us(void)
{
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}
//产生一个us秒级延时
void Delay_nus(uint32_t time)
{
	while(time--)
	{
		Delay_1us();
	}
}

//这个就是一个精准的系统滴答的延时函数

void Delay_ms(uint32_t time)
{
	systime=0;
	while(systime<time) ++ systime;
}

static u32 nTicks = 0;
u32 getTicks(){		//获取系统执行时间,nTicks节拍数
	return nTicks;
}

//uint16_t ledrun[2]={0,500};
//中断服务函数可以不声明  不调用
//你想放到哪里都行  除了汇编文件
void SysTick_Handler(void)//这个就是系统滴答的中断服务函数
{
	//1ms进一次
	systime++;  //也就是说我这个变量无论在什么情况下  它都会1ms自
	//ledrun[0]++;
}
