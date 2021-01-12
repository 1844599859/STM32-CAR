#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f10x.h"
void Init_Delay(void);
void Delay_ms(uint32_t time);
void Delay_nus(uint32_t time);
u32 getTicks();
#endif
