#include "delay.h"
uint32_t  systime=0;

void Init_Delay(void)
{
	SysTick_Config(72000-1);//ϵͳ�δ�ʱ���ͳ�ʼ�������
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
//����һ��us�뼶��ʱ
void Delay_nus(uint32_t time)
{
	while(time--)
	{
		Delay_1us();
	}
}

//�������һ����׼��ϵͳ�δ����ʱ����

void Delay_ms(uint32_t time)
{
	systime=0;
	while(systime<time) ++ systime;
}

static u32 nTicks = 0;
u32 getTicks(){		//��ȡϵͳִ��ʱ��,nTicks������
	return nTicks;
}

//uint16_t ledrun[2]={0,500};
//�жϷ��������Բ�����  ������
//����ŵ����ﶼ��  ���˻���ļ�
void SysTick_Handler(void)//�������ϵͳ�δ���жϷ�����
{
	//1ms��һ��
	systime++;  //Ҳ����˵���������������ʲô�����  ������1ms��
	//ledrun[0]++;
}
