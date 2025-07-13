#ifndef _Key__H
#define _Key__H

void GPIO_KeyInit(void);
void Timer_Init(void);
uint8_t Key(void);
uint8_t Get_Key(void);
void Loop_Key(void);
void Key_Init(void);
void TIM2_IRQHandler(void);

#endif
