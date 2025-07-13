#ifndef __BOX_ACTION_H
#define __BOX_ACTION_H

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "PWM.h"
#include "Key.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 设置开合PWM值 */
#define LID_OPEN_VALUE       110
#define LID_NO_OPEN_VALUE    132
#define LID_CLOSE_VALUE      180
#define PUSH_OPEN_VALUE      175

#define PUSH_CLOSE_VALUE     40
#define LID_NO_CLOSE_VALUE   145
#define PUSH_NO_OPEN_VALUE   120
#define PUSH_FAKE_OPEN_VALUE 120

/* 舵机选择宏 */
#define F_LID                0
#define F_PUSH               1

/* 动作类型宏 */
#define OPEN                 0
#define CLOSE                1
#define PUSH                 2
#define FOLD                 3
#define NO_CLOSE             4
#define NO_PUSH              5
#define NO_OPEN              6
#define NIL                  8

/* 动作宏定义 */
#define LID_OPEN             Servo_SetAngle2(LID_OPEN_VALUE)
#define LID_CLOSE            Servo_SetAngle2(LID_CLOSE_VALUE)
#define PUSH_OPEN           Servo_SetAngle1(PUSH_OPEN_VALUE)
#define PUSH_CLOSE          Servo_SetAngle1(PUSH_CLOSE_VALUE)

#define LID_NO_OPEN          Servo_SetAngle2(LID_NO_OPEN_VALUE)
#define LID_NO_CLOSE         Servo_SetAngle2(LID_NO_CLOSE_VALUE)
#define PUSH_NO_OPEN         Servo_SetAngle1(PUSH_NO_OPEN_VALUE)

/* 时间宏定义 */
#define Short_time           800
#define Long_time            3000

/* 全局变量声明 */
extern uint8_t Cnt;
extern uint8_t mode;
extern const uint8_t mode_num;
extern uint8_t random_time;
extern _Bool Action;

/* 函数声明 */
void Action_Type(uint32_t windup, uint8_t action, uint32_t recovery);
void Action_Slow(uint32_t windup, _Bool part, uint8_t begin, uint8_t final, uint8_t change, uint32_t time, uint32_t recovery);
void Box_action(void);
void Delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __BOX_ACTION_H */