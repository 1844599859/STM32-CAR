#ifndef _CS_H
#define _CS_H

//#include "sys.h"
#define uint unsigned int
#define TRIG_Send  PBout(8)
#define ECHO_Reci  PBin(9)

void CH_SR04_Init(void);
float Sensor_Using(void);
void Sensor_NVIC_Config(void);
#endif
