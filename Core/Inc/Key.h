#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 按键相关函数声明 */
void GPIO_KeyInit(void);
void Timer_Init(void);
uint8_t Key(void);
uint8_t Get_Key(void);
void Loop_Key(void);
void Key_Init(void);
void TIM2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */