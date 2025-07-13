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
        case 0: LID_OPEN; break;
        case 1: LID_CLOSE; break;
        case 2: PUSH_OPEN; break;
        case 3: PUSH_CLOSE; break;
        case 4: LID_NO_CLOSE; break;
        case 5: PUSH_NO_OPEN; break;
        case 6: LID_NO_OPEN; break;
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
            Action_Type(Short_time, OPEN, 900);
            Action_Type(0, PUSH, 700);
            Action_Type(0, FOLD, 500);
            Action_Type(0, CLOSE, 100);
            break;
            
        case 1: // 开盖、推杆、收杆、关盖
            Action_Type(Short_time, OPEN, 500);
            Action_Type(0, PUSH, 400);
            Action_Type(0, FOLD, 200);
            Action_Type(0, CLOSE, 80);
            break;
            
        case 2: // 缓慢开盖，推杆移动到钮子前，收杆，推杆，收杆，关盖
            Action_Slow(Short_time, F_LID, LID_CLOSE_VALUE, LID_OPEN_VALUE, 2, 50, 1000);
            Action_Slow(0, F_PUSH, PUSH_CLOSE_VALUE, PUSH_FAKE_OPEN_VALUE, 2, 10, 1000);
            Action_Type(0, FOLD, 50);
            Action_Type(0, PUSH, 260);
            Action_Type(0, FOLD, 220);
            Action_Type(0, CLOSE, 300);
            break;
            
        case 3: // 缓慢开盖，关盖，开盖，推杆，收杆，关盖
            Action_Slow(Long_time, F_LID, LID_CLOSE_VALUE, LID_OPEN_VALUE, 2, 50, 1600);
            Action_Slow(0, F_LID, LID_OPEN_VALUE, LID_CLOSE_VALUE, 2, 50, 2000);
            Action_Type(0, OPEN, 250);
            Action_Type(0, PUSH, 260);
            Action_Type(0, FOLD, 220);
            Action_Type(0, CLOSE, 50);
            break;
            
        case 4: // 开盖、推杆、收杆、关盖
            Action_Type(Short_time, OPEN, 100);
            Action_Type(0, PUSH, 280);
            Action_Type(0, FOLD, 200);
            Action_Type(0, CLOSE, 50);
            break;
            
        case 5: // 开盖，连续推杆，收杆，关盖，开盖，推杆，收杆，关盖
            Action_Type(Short_time, OPEN, 500);
            Action_Slow(0, F_PUSH, PUSH_CLOSE_VALUE, PUSH_FAKE_OPEN_VALUE, 2, 10, 1600);
            for(uint8_t i = 0; i < 8; i++)
            {
                Action_Type(0, FOLD, 80);
                Action_Type(0, PUSH, 80);
            }
            Action_Type(500, NO_CLOSE, 100);
            Action_Type(2000, OPEN, 180);
            Action_Type(0, FOLD, 100);
            Action_Type(0, CLOSE, 100);
            break;
            
        case 6: // 开盖，推杆，扑打盖，收杆，关盖
            Action_Type(Short_time, OPEN, 500);
            Action_Type(0, PUSH, 500);
            for(uint8_t i = 0; i < 12; i++)
            {
                Action_Type(0, NO_CLOSE, 80);
                Action_Type(0, OPEN, 100);
            }
            Action_Type(300, FOLD, 600);
            Action_Type(0, CLOSE, 50);
            break;
            
        case 7: // 开盖，关盖，开盖，推杆，收杆，关盖
            Action_Type(Short_time, OPEN, 500);
            Action_Type(0, CLOSE, 300);
            Action_Type(0, OPEN, 1200);
            Action_Type(0, PUSH, 400);
            Action_Type(0, FOLD, 200);
            Action_Type(0, CLOSE, 300);
            break;
            
        case 8: // 开盖，颤抖盖，连续推杆，收杆，关盖
            Action_Slow(Long_time, F_LID, LID_CLOSE_VALUE, LID_OPEN_VALUE, 2, 30, 500);
            for(uint8_t i = 0; i < 18; i++)
            {
                Action_Type(0, NO_CLOSE, 40);
                Action_Type(0, OPEN, 40);
            }
            Action_Type(0, PUSH, 100);
            for(uint8_t i = 0; i < 10; i++)
            {
                Action_Type(0, PUSH, 150);
                Action_Type(0, FOLD, 50);
            }
            Action_Type(0, CLOSE, 40);
            break;
            
        case 9: // 开盖，关盖
            Action_Type(Short_time, OPEN, 1500);
            Action_Type(0, CLOSE, 200);
            break;
            
        case 10: // 开盖，关盖，开盖，推杆，收杆，关盖
            Action_Type(Short_time, OPEN, 1500);
            Action_Type(0, CLOSE, 2000);
            Action_Type(0, OPEN, 200);
            Action_Type(0, PUSH, 260);
            Action_Type(0, FOLD, 200);
            Action_Type(0, CLOSE, 300);
            break;
            
        // 继续添加其他case...
        default: // 开盖，推杆，收杆，关盖
            Action_Type(Short_time, OPEN, 100);
            Action_Type(0, PUSH, 280);
            Action_Type(0, FOLD, 200);
            Action_Type(0, CLOSE, 50);
            break;
    }
    
    Action = 0;
    Delay_ms(300);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
}