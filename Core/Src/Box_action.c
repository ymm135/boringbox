#include "Box_action.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/* 外部变量声明 */
extern TIM_HandleTypeDef htim4;

/* 全局变量定义 */
uint8_t Cnt = 0;
uint8_t mode = 0;
uint8_t random_time;
_Bool Action;
const uint8_t mode_num = 24;

/* 待机模式相关变量 */
uint32_t last_activity_time = 0;  // 最后活动时间
uint8_t system_state = SYSTEM_ACTIVE;  // 系统状态，默认为活跃状态

/**
 * @brief 延时函数
 * @param ms 延时毫秒数
 * @retval None
 */
void Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
 * @brief 简易动作（单纯开关）：用于推杆、盖子简单的开合动作
 * @param windup   释放前的等待时间
 * @param action   释放的动作类型
 * @param recovery 释放后的等待时间
 * @retval None
 */
void Action_Type(uint32_t windup, uint8_t action, uint32_t recovery)
{
    Delay_ms(windup);
    switch(action)
    {
        case TYPE_LID_OPEN: LID_OPEN; break;
        case TYPE_LID_CLOSE: LID_CLOSE; break;
        case TYPE_PUSH_OPEN: PUSH_OPEN; break;
        case TYPE_PUSH_CLOSE: PUSH_CLOSE; break;
        case TYPE_LID_NO_CLOSE: LID_NO_CLOSE; break;
        case TYPE_PUSH_NO_OPEN: PUSH_NO_OPEN; break;
        case TYPE_LID_NO_OPEN: LID_NO_OPEN; break;
        default: break;
    }
    Delay_ms(recovery);
}

/**
 * @brief 缓慢动作函数
 * @param windup   前摇时间
 * @param part     舵机选择 (F_LID或F_PUSH)
 * @param begin    起始角度
 * @param final    结束角度
 * @param change   每次变化的角度
 * @param time     每步的延时
 * @param recovery 后摇时间
 * @retval None
 */
void Action_Slow(uint32_t windup, _Bool part, uint8_t begin, uint8_t final, uint8_t change, uint32_t time, uint32_t recovery)
{
    Delay_ms(windup);
    if(part) // PUSH
    {
        if(begin < final)
        {
            for(uint8_t i = begin; i < final; i += change)
            {
                Servo_SetAngle1(i);
                Delay_ms(time);
            }
        }
        else
        {
            for(uint8_t i = begin; i > final; i -= change)
            {
                Servo_SetAngle1(i);
                Delay_ms(time);
            }
        }
    }
    else // LID
    {
        if(begin < final)
        {
            for(uint8_t i = begin; i < final; i += change)
            {
                Servo_SetAngle2(i);
                Delay_ms(time);
            }
        }
        else
        {
            for(uint8_t i = begin; i > final; i -= change)
            {
                Servo_SetAngle2(i);
                Delay_ms(time);
            }
        }
    }
    Delay_ms(recovery);
}

/**
 * @brief 盒子动作主函数
 * @param None
 * @retval None
 */
void Box_action(void)
{
    Action = 1;
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    
    switch(mode)
    {
        case 0: // 开盖、推杆、收杆、关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 900);
            Action_Type(0, TYPE_PUSH_OPEN, 700);
            Action_Type(0, TYPE_PUSH_CLOSE, 500);
            Action_Type(0, TYPE_LID_CLOSE, 100);
            break;
            
        case 1: // 开盖、推杆、收杆、关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 500);
            Action_Type(0, TYPE_PUSH_OPEN, 400);
            Action_Type(0, TYPE_PUSH_CLOSE, 200);
            Action_Type(0, TYPE_LID_CLOSE, 80);
            break;
            
        case 2: // 缓慢开盖，推杆移动到钮子前，收杆，推杆，收杆，关盖
            Action_Slow(Short_time, F_LID, LID_CLOSE_VALUE, LID_OPEN_VALUE, 2, 50, 1000);
            Action_Slow(0, F_PUSH, PUSH_CLOSE_VALUE, PUSH_FAKE_OPEN_VALUE, 2, 10, 1000);
            Action_Type(0, TYPE_PUSH_CLOSE, 50);
            Action_Type(0, TYPE_PUSH_OPEN, 260);
            Action_Type(0, TYPE_PUSH_CLOSE, 220);
            Action_Type(0, TYPE_LID_CLOSE, 300);
            break;
            
        case 3: // 缓慢开盖，关盖，开盖，推杆，收杆，关盖
            Action_Slow(Long_time, F_LID, LID_CLOSE_VALUE, LID_OPEN_VALUE, 2, 50, 1600);
            Action_Slow(0, F_LID, LID_OPEN_VALUE, LID_CLOSE_VALUE, 2, 50, 2000);
            Action_Type(0, TYPE_LID_OPEN, 250);
            Action_Type(0, TYPE_PUSH_OPEN, 260);
            Action_Type(0, TYPE_PUSH_CLOSE, 220);
            Action_Type(0, TYPE_LID_CLOSE, 50);
            break;
            
        case 4: // 开盖、推杆、收杆、关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 100);
            Action_Type(0, TYPE_PUSH_OPEN, 280);
            Action_Type(0, TYPE_PUSH_CLOSE, 200);
            Action_Type(0, TYPE_LID_CLOSE, 50);
            break;
            
        case 5: // 开盖，连续推杆，收杆，关盖，开盖，推杆，收杆，关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 500);
            Action_Slow(0, F_PUSH, PUSH_CLOSE_VALUE, PUSH_FAKE_OPEN_VALUE, 2, 10, 1600);
            for(uint8_t i = 0; i < 8; i++)
            {
                Action_Type(0, TYPE_PUSH_CLOSE, 80);
                Action_Type(0, TYPE_PUSH_OPEN, 80);
            }
            Action_Type(500, TYPE_LID_NO_CLOSE, 100);
            Action_Type(2000, TYPE_LID_OPEN, 180);
            Action_Type(0, TYPE_PUSH_CLOSE, 100);
            Action_Type(0, TYPE_LID_CLOSE, 100);
            break;
            
        case 6: // 开盖，推杆，扑打盖，收杆，关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 500);
            Action_Type(0, TYPE_PUSH_OPEN, 500);
            for(uint8_t i = 0; i < 12; i++)
            {
                Action_Type(0, TYPE_LID_NO_CLOSE, 80);
                Action_Type(0, TYPE_LID_OPEN, 100);
            }
            Action_Type(300, TYPE_PUSH_CLOSE, 600);
            Action_Type(0, TYPE_LID_CLOSE, 50);
            break;
            
        case 7: // 开盖，关盖，开盖，推杆，收杆，关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 500);
            Action_Type(0, TYPE_LID_CLOSE, 300);
            Action_Type(0, TYPE_LID_OPEN, 1200);
            Action_Type(0, TYPE_PUSH_OPEN, 400);
            Action_Type(0, TYPE_PUSH_CLOSE, 200);
            Action_Type(0, TYPE_LID_CLOSE, 300);
            break;
            
        case 8: // 开盖，颤抖盖，连续推杆，收杆，关盖
            Action_Slow(Long_time, F_LID, LID_CLOSE_VALUE, LID_OPEN_VALUE, 2, 30, 500);
            for(uint8_t i = 0; i < 18; i++)
            {
                Action_Type(0, TYPE_LID_NO_CLOSE, 40);
                Action_Type(0, TYPE_LID_OPEN, 40);
            }
            Action_Type(0, TYPE_PUSH_OPEN, 100);
            for(uint8_t i = 0; i < 10; i++)
            {
                Action_Type(0, TYPE_PUSH_OPEN, 150);
                Action_Type(0, TYPE_PUSH_CLOSE, 50);
            }
            Action_Type(0, TYPE_LID_CLOSE, 40);
            break;
            
        case 9: // 开盖，关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 1500);
            Action_Type(0, TYPE_LID_CLOSE, 200);
            break;
            
        case 10: // 开盖，关盖，开盖，推杆，收杆，关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 1500);
            Action_Type(0, TYPE_LID_CLOSE, 2000);
            Action_Type(0, TYPE_LID_OPEN, 200);
            Action_Type(0, TYPE_PUSH_OPEN, 260);
            Action_Type(0, TYPE_PUSH_CLOSE, 200);
            Action_Type(0, TYPE_LID_CLOSE, 300);
            break;
            
        // 继续添加其他case...
        default: // 开盖，推杆，收杆，关盖
            Action_Type(Short_time, TYPE_LID_OPEN, 100);
            Action_Type(0, TYPE_PUSH_OPEN, 280);
            Action_Type(0, TYPE_PUSH_CLOSE, 200);
            Action_Type(0, TYPE_LID_CLOSE, 50);
            break;
    }
    
    Action = 0;
    Delay_ms(300);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
}

/**
 * @brief 更新活动时间
 * @param None
 * @retval None
 */
void Update_Activity_Time(void)
{
    last_activity_time = HAL_GetTick();
}

/**
 * @brief 检查待机超时
 * @param None
 * @retval None
 */
void Check_Standby_Timeout(void)
{
    if(system_state == SYSTEM_ACTIVE)
    {
        uint32_t current_time = HAL_GetTick();
        if((current_time - last_activity_time) >= STANDBY_TIMEOUT)
        {
            Enter_Standby_Mode();
        }
    }
}

/**
 * @brief 进入待机模式
 * @param None
 * @retval None
 */
void Enter_Standby_Mode(void)
{
    system_state = SYSTEM_STANDBY;
    
    // 停止PWM输出
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    
    // 配置PA2为外部中断唤醒源
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  // 下降沿触发
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 使能EXTI2中断
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    
    // 进入停止模式
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

/**
 * @brief 退出待机模式
 * @param None
 * @retval None
 */
void Exit_Standby_Mode(void)
{
    // 重新配置系统时钟
    SystemClock_Config();
    
    // 重新配置PA2为普通输入模式
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 禁用EXTI2中断
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    
    system_state = SYSTEM_ACTIVE;
    Update_Activity_Time();
}

/**
 * @brief GPIO外部中断回调函数
 * @param GPIO_Pin: 触发中断的GPIO引脚
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_2 && system_state == SYSTEM_STANDBY)
    {
        Exit_Standby_Mode();
    }
}