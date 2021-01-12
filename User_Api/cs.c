#include "cs.h"
#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>


static void delay(int ms)
{
	for(int i=0;i<ms;i++)
		for(int j=0;j<1000;j++);
}

static void delay_us(int us)
{
	for(int i=0;i<us;i++);
}


/*记录定时器溢出次数*/
uint overcount=0;

/*设置中断优先级*/
void Sensor_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructer;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=4;
	NVIC_InitStructer.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;

	NVIC_Init(&NVIC_InitStructer);
}

/*初始化模块的GPIO以及初始化定时器TIM2*/
void CH_SR04_Init(void)
{
	// 结构体初始化
	GPIO_InitTypeDef GPIO_InitStructer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	// 定时器使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/*TRIG触发信号 PE1 PP 50MHz*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructer);

	/*ECOH回响信号 PE2 FLOATING*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOE, &GPIO_InitStructer);

	/*定时器TIM2初始化*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructer.TIM_Period=999;//定时周期为1000
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; //分频系数72
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//开启更新中断

	Sensor_NVIC_Config();
	TIM_Cmd(TIM2,DISABLE);//关闭定时器使能
}

float Sensor_Using(void)
{
	float length=0,sum=0;
	u16 tim;
	uint i=0;
	
	GPIO_SetBits(GPIOE, GPIO_Pin_1);
	delay(1000);  //高电平信号超过10us
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==RESET);
	TIM_Cmd(TIM2,ENABLE);//回响信号到来，开启定时器计数
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==SET);//回响信号消失
	TIM_Cmd(TIM2,DISABLE);//关闭定时器
	tim=TIM_GetCounter(TIM2);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间
	printf("%d",tim);
	length=(tim+overcount*1000)/58.0;//通过回响信号计算距离
	TIM2->CNT=0;
	overcount=0;
	delay(100);
	return length;
	
//	/*测5次数据计算一次平均值*/
//	while(i!=5)
//	{
//		GPIO_SetBits(GPIOE, GPIO_Pin_1);
//		delay(1000);  //高电平信号超过10us
//		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
//		/*等待回响信号*/
//		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==RESET);
//		TIM_Cmd(TIM2,ENABLE);//回响信号到来，开启定时器计数

//		i+=1; //每收到一次回响信号+1，收到5次就计算均值
//		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==SET);//回响信号消失
//		TIM_Cmd(TIM2,DISABLE);//关闭定时器

//		tim=TIM_GetCounter(TIM2);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间

//		length=(tim+overcount*1000)/58.0;//通过回响信号计算距离

//		sum=length+sum;
//		TIM2->CNT=0;  //将TIM2计数寄存器的计数值清零
//		overcount=0;  //中断溢出次数清零
//		delay(100);
//	}
//	length=sum/5;
//	return length;//距离作为函数返回值
}


void TIM2_IRQHandler(void) //中断，当回响信号很长是，计数值溢出后重复计数，用中断来保存溢出次数
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志
		overcount++;
	}
}
