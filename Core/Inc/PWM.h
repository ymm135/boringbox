#ifndef __PWM_H
#define __PWM_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* PWM配置宏定义 */
#define PWM_PERIOD_VALUE    1999        // 20ms周期 (50Hz)
#define PWM_PRESCALER_VALUE 719         // 72MHz/720 = 100kHz
#define SERVO_MIN_PULSE     50          // 0.5ms (50/100kHz = 0.5ms)
#define SERVO_MAX_PULSE     250         // 2.5ms (250/100kHz = 2.5ms) 
#define SERVO_CENTER_PULSE  150         // 1.5ms (150/100kHz = 1.5ms)
#define PWM_CHANNEL_1       1
#define PWM_CHANNEL_2       2

/* PWM状态枚举 */
typedef enum {
    PWM_OK = 0,
    PWM_ERROR_INVALID_CHANNEL,
    PWM_ERROR_INVALID_PARAMETER
} PWM_StatusTypeDef;

/* 函数声明 */
void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void Servo_SetAngle1(float Angle1);
void Servo_SetAngle2(float Angle2);
PWM_StatusTypeDef PWM_SetDutyCycle(uint8_t channel, float duty_cycle);
PWM_StatusTypeDef Servo_SetAngle(uint8_t channel, float angle);
PWM_StatusTypeDef Servo_SetAngleWithLimit(uint8_t channel, float angle, float min_angle, float max_angle);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_H */