#include "PWM.h"

extern TIM_HandleTypeDef htim4;

void PWM_Init(void)
{
    // TIM4已经在main.c中初始化，这里只需要启动PWM
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

void PWM_SetCompare1(uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, Compare);
}

void PWM_SetCompare2(uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, Compare);
}

void Servo_SetAngle1(float Angle1)
{
    uint16_t pulse = (uint16_t)(SERVO_MIN_PULSE + (Angle1 / 180.0f) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE));
    PWM_SetCompare1(pulse);
}

void Servo_SetAngle2(float Angle2)
{
    uint16_t pulse = (uint16_t)(SERVO_MIN_PULSE + (Angle2 / 180.0f) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE));
    PWM_SetCompare2(pulse);
}

// 为了保持接口兼容性，保留这些函数但使用简化实现
PWM_StatusTypeDef PWM_SetDutyCycle(uint8_t channel, float duty_cycle)
{
    uint32_t pulse_value = (uint32_t)((duty_cycle / 100.0f) * (PWM_PERIOD_VALUE + 1));
    
    if (channel == PWM_CHANNEL_1)
    {
        PWM_SetCompare1(pulse_value);
    }
    else if (channel == PWM_CHANNEL_2)
    {
        PWM_SetCompare2(pulse_value);
    }
    
    return PWM_OK;
}

PWM_StatusTypeDef Servo_SetAngle(uint8_t channel, float angle)
{
    if (channel == PWM_CHANNEL_1)
    {
        Servo_SetAngle1(angle);
    }
    else if (channel == PWM_CHANNEL_2)
    {
        Servo_SetAngle2(angle);
    }
    
    return PWM_OK;
}

PWM_StatusTypeDef Servo_SetAngleWithLimit(uint8_t channel, float angle, float min_angle, float max_angle)
{
    // 限制角度范围
    if (angle < min_angle) angle = min_angle;
    if (angle > max_angle) angle = max_angle;
    
    return Servo_SetAngle(channel, angle);
}