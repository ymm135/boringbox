#include "PWM.h"
#include "main.h"

/* 外部变量声明 */
extern TIM_HandleTypeDef htim4;

/**
 * @brief PWM初始化函数
 * @param None
 * @retval None
 */
void PWM_Init(void)
{
  // 启动PWM输出
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  // PB6 - PWM1
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);  // PB7 - PWM2
}

/**
 * @brief 设置PWM通道1的比较值
 * @param Compare 比较值
 * @retval None
 */
void PWM_SetCompare1(uint16_t Compare)
{
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, Compare);
}

/**
 * @brief 设置PWM通道2的比较值
 * @param Compare 比较值
 * @retval None
 */
void PWM_SetCompare2(uint16_t Compare)
{
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, Compare);
}

/**
 * @brief 设置舵机1的角度
 * @param Angle1 角度值 (0-180度)
 * @retval None
 */
void Servo_SetAngle1(float Angle1)
{
  // 限制角度范围
  if (Angle1 < 0.0f) Angle1 = 0.0f;
  if (Angle1 > 180.0f) Angle1 = 180.0f;
  
  // 计算脉冲值: 0.5ms(0度) 到 2.5ms(180度)
  uint32_t pulse_value = SERVO_MIN_PULSE + (uint32_t)((Angle1 / 180.0f) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE));
  PWM_SetCompare1(pulse_value);
}

/**
 * @brief 设置舵机2的角度
 * @param Angle2 角度值 (0-180度)
 * @retval None
 */
void Servo_SetAngle2(float Angle2)
{
  // 限制角度范围
  if (Angle2 < 0.0f) Angle2 = 0.0f;
  if (Angle2 > 180.0f) Angle2 = 180.0f;
  
  // 计算脉冲值: 0.5ms(0度) 到 2.5ms(180度)
  uint32_t pulse_value = SERVO_MIN_PULSE + (uint32_t)((Angle2 / 180.0f) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE));
  PWM_SetCompare2(pulse_value);
}

/**
 * @brief 设置PWM占空比
 * @param channel PWM通道 (1或2)
 * @param duty_cycle 占空比 (0-100)
 * @retval PWM_StatusTypeDef 操作状态
 */
PWM_StatusTypeDef PWM_SetDutyCycle(uint8_t channel, float duty_cycle)
{
  uint32_t pulse_value;

  // 参数验证
  if (channel != PWM_CHANNEL_1 && channel != PWM_CHANNEL_2) {
    return PWM_ERROR_INVALID_CHANNEL;
  }

  if (duty_cycle < 0.0f || duty_cycle > 100.0f) {
    return PWM_ERROR_INVALID_PARAMETER;
  }

  // 计算脉冲值
  pulse_value = (uint32_t)((duty_cycle / 100.0f) * (PWM_PERIOD_VALUE + 1));

  if (channel == PWM_CHANNEL_1)
  {
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pulse_value);
  }
  else if (channel == PWM_CHANNEL_2)
  {
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pulse_value);
  }

  return PWM_OK;
}

/**
 * @brief 设置舵机角度 (适用于标准舵机)
 * @param channel PWM通道 (1或2)
 * @param angle 角度 (0-180度)
 * @retval PWM_StatusTypeDef 操作状态
 */
PWM_StatusTypeDef Servo_SetAngle(uint8_t channel, float angle)
{
  // 参数验证
  if (channel != PWM_CHANNEL_1 && channel != PWM_CHANNEL_2) {
    return PWM_ERROR_INVALID_CHANNEL;
  }

  if (angle < 0.0f || angle > 180.0f) {
    return PWM_ERROR_INVALID_PARAMETER;
  }

  // 标准舵机: 0.5ms(0度) 到 2.5ms(180度), 周期20ms
  // 脉冲值: SERVO_MIN_PULSE 到 SERVO_MAX_PULSE
  uint32_t pulse_value = SERVO_MIN_PULSE + (uint32_t)((angle / 180.0f) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE));

  if (channel == PWM_CHANNEL_1)
  {
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pulse_value);
  }
  else if (channel == PWM_CHANNEL_2)
  {
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pulse_value);
  }

  return PWM_OK;
}

/**
 * @brief 设置舵机角度 (带限位保护)
 * @param channel PWM通道 (1或2)
 * @param angle 目标角度
 * @param min_angle 最小角度限制
 * @param max_angle 最大角度限制
 * @retval PWM_StatusTypeDef 操作状态
 */
PWM_StatusTypeDef Servo_SetAngleWithLimit(uint8_t channel, float angle, float min_angle, float max_angle)
{
  // 参数验证
  if (channel != PWM_CHANNEL_1 && channel != PWM_CHANNEL_2) {
    return PWM_ERROR_INVALID_CHANNEL;
  }

  if (min_angle < 0.0f || max_angle > 180.0f || min_angle >= max_angle) {
    return PWM_ERROR_INVALID_PARAMETER;
  }

  // 限制角度范围
  if (angle < min_angle) angle = min_angle;
  if (angle > max_angle) angle = max_angle;

  return Servo_SetAngle(channel, angle);
}