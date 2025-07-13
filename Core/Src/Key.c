#include "Key.h"
#include "main.h"

/* 外部变量声明 */
extern TIM_HandleTypeDef htim2;

/* 私有变量 */
static uint8_t Key_KeyNumber = 0;

/**
 * @brief GPIO按键初始化
 * @param None
 * @retval None
 */
void GPIO_KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* Configure GPIO pin : PA2 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief 定时器初始化（用于按键扫描）
 * @param None
 * @retval None
 */
void Timer_Init(void)
{
    /* TIM2 已在CubeMX中配置，这里启动定时器中断 */
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief 获取按键值
 * @param None
 * @retval uint8_t 按键值
 */
uint8_t Key(void)
{
    uint8_t Temp;
    Temp = Key_KeyNumber;
    Key_KeyNumber = 0;
    return Temp;
}

/**
 * @brief 获取按键电平状态
 * @param None
 * @retval uint8_t 按键状态 (1-按下, 0-释放)
 */
uint8_t Get_Key(void)
{
    uint8_t KeyNum = 0;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
        KeyNum = 1;
    }
    return KeyNum;
}

/**
 * @brief 按键扫描循环
 * @param None
 * @retval None
 */
void Loop_Key(void)
{
    static uint8_t LastKey = 0, NowKey = 0;
    LastKey = NowKey;
    NowKey = Get_Key();
    if (LastKey == 0 && NowKey == 1)
    {
        Key_KeyNumber = 1;
    }
}

/**
 * @brief 按键初始化
 * @param None
 * @retval None
 */
void Key_Init(void)
{
    GPIO_KeyInit();
    Timer_Init();
}

/**
 * @brief HAL库定时器中断回调函数
 * @param htim 定时器句柄
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        static uint8_t scan_time = 0;
        
        scan_time++;
        if (scan_time >= 1)  // 每次中断都执行按键扫描
        {
            scan_time = 0;
            Loop_Key();
        }
    }
}