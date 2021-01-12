#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
#include "led.h"
#include "cry.h"
#include "cs.h"
#include "car.h"

u32 num = 16;
u32 h;

void delay(int ms)
{
	for(int i=0;i<ms;i++)
		for(int j=0;j<10000;j++);
}

//GPIO初始化
//使能A口，4，5，6，7推挽输出10MHz
void GPIOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //重置
	//A口0，1，2，3上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

//中断初始化，主优先级为0，子优先级分别为0，1，2，3；线选的0，1，2，3
void NVICInit(void)   
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
//外部中断事件控制器初始化
void EXTIInit()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIOInit();
	NVICInit();
	//RCC使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//Selects the GPIO pin used as EXTI Line，相互绑定
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)地址映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3; //中断事件线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //EXTI模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发类型·
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//EXTI使能
	EXTI_Init(&EXTI_InitStructure);  //初始化
}

void EXTI0_3_disable()//关闭
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void EXTI0_3_enable()//开启
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void EXTI0_IRQHandler()//中断处理程序
{
		u8 i;
	  Delay_ms(50);
	
	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0)
	 {
			EXTI0_3_disable();
			for(i=4;i<8;i++){
				GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
				GPIOA->ODR &= ~(1 << i);
				Delay_ms(50);
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0){
					switch(i){
						case(4):
							num=12;
							break;
						case(5): 
							num=13;
							break;
						case(6): 
							num=14;
							break;
						case(7):
							num=15;
							break;
					default:num=16;
					} 
				}				
			}
		if (num != 16)
		{
			GPIOE->ODR = 0xff00;
			GPIOE->ODR &= ~(1 << 8);
			GPIOE->ODR &= ~(1 << num);
			printf("key num: 4 %d",num%10-1);
			Cry(500);
		}
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		EXTI0_3_enable();
	}

	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler()
{
		u8 i;
	  Delay_ms(50);
	
	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)
	 {
			EXTI0_3_disable();
			for(i=4;i<8;i++){
				GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
				GPIOA->ODR &= ~(1 << i);
				Delay_ms(50);
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0){
					switch(i){
						case(4):
							num=12;
							break;
						case(5): 
							num=13;
							break;
						case(6): 
							num=14;
							break;
						case(7):
							num=15;
							break;
					default:num=16;
					} 
				}				
			}
		if (num != 16)
		{
			GPIOE->ODR = 0xff00;
			GPIOE->ODR &= ~(1 << 9);
			GPIOE->ODR &= ~(1 << num);
			printf("key num: 3 %d",num%10-1);
			Cry(500);
		}
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		EXTI0_3_enable();
	}

	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_IRQHandler()
{
		u8 i;
	  Delay_ms(50);
	
	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0)
	 {
			EXTI0_3_disable();
			for(i=4;i<8;i++){
				GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
				GPIOA->ODR &= ~(1 << i);
				Delay_ms(50);
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0){
					switch(i){
						case(4):
							num=12;
							break;
						case(5): 
							num=13;
							break;
						case(6): 
							num=14;
							break;
						case(7):
							num=15;
							break;
					default:num=16;
					} 
				}				
			}
		if (num != 16)
		{
			GPIOE->ODR = 0xff00;
			GPIOE->ODR &= ~(1 << 10);
			GPIOE->ODR &= ~(1 << num);
			printf("key num: 2 %d",num%10-1);
			Cry(500);
		}
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		EXTI0_3_enable();
	}

	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler()
{
		u8 i;
	  Delay_ms(50);
	
	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==0)
	 {
		 EXTI0_3_disable();//关闭，不会再有异常产生
			for(i=4;i<8;i++){
				GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
				GPIOA->ODR &= ~(1 << i);//置0打开
				Delay_ms(50);
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==0){
					switch(i){
						case(4):
							num=12;
							break;
						case(5): 
							num=13;
							break;
						case(6): 
							num=14;
							break;
						case(7):
							num=15;
							break;
					default:num=16;
					} 
				}				
			}
		if (num != 16)
		{
			GPIOE->ODR = 0xff00;
			GPIOE->ODR &= ~(1 << 11);
			GPIOE->ODR &= ~(1 << num);
			printf("key num: 1 %d",num%10-1);
			Cry(500);
			
		}
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		EXTI0_3_enable();//开启
	}

	EXTI_ClearITPendingBit(EXTI_Line3);
}

void USART1_Init(unsigned int BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);			//??USART1,GPIOA??
	
	/* TX - PA.9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;							//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;							//??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* RX - PA.10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;							//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = BaudRate;							//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;					//??8?
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						//???1?
	USART_InitStructure.USART_Parity = USART_Parity_No;						//?????
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;		//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//?/???
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);							//??????
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);							//??????
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
	static unsigned char buff[64];
	static unsigned char n = 0;
	unsigned char i;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//?????????
	{
		buff[n++] = USART1->DR;																//??????????
 
		if(n == 64)
		{
			n = 0;
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)		//?????????
	{
		USART1->DR;						//?DR,???
		
		printf("%d characters:\r\n", n);
		for(i=0; i<n; i++)
		{
			printf("buff[%d] = 0x%02hhx\r\n", i, buff[i]);	//??????,??????,???0
		}
		n = 0;
	}
}

int fputc(int ch, FILE *f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		//????????
	USART_SendData(USART1, (unsigned char)ch);				//???????
	return ch;
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//????1:1??????,3??????
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//????
	NVIC_Init(&NVIC_InitStructure);
}

int main()
{
	float distance;
	Init_LED();								// 初始化LED
	Init_Cry();								// 初始化蜂鸣器
	EXTIInit();								// 初始化按键中断
	USART1_Init(115200);
	NVIC_Config();
	CH_SR04_Init();						// 初始化测速
	Init_Car();								// 小车初始化
	printf("Welcome to use our AI Car!");
	Cry(500);
	while(1)
	{
		distance = Sensor_Using();
		printf("The distance is :%.3fCM", distance);
		if(distance<=20.0)
		{
			Car_Stop();
			Cry(500);
			delay(100);
			Cry(500);
			delay(100);
			Cry(500);
			Stop_Led();
		}
		else 
		{
			Car_Run();
			Run_Led();
		}
	}
}

