# PWM模块使用说明

## 概述

本项目已将PWM相关功能从main.c中独立出来，形成了独立的PWM模块。PWM模块提供了与开源代码兼容的接口，支持舵机控制和通用PWM输出。

## 文件结构

- `Core/Inc/PWM.h` - PWM模块头文件
- `Core/Src/PWM.c` - PWM模块源文件

## 功能特性

### 1. 兼容开源代码接口

提供与参考代码相同的函数接口：

```c
void PWM_Init(void);                    // PWM初始化
void PWM_SetCompare1(uint16_t Compare); // 设置通道1比较值
void PWM_SetCompare2(uint16_t Compare); // 设置通道2比较值
void Servo_SetAngle1(float Angle1);     // 设置舵机1角度
void Servo_SetAngle2(float Angle2);     // 设置舵机2角度
```

### 2. 增强功能接口

提供更完善的错误处理和参数验证：

```c
PWM_StatusTypeDef PWM_SetDutyCycle(uint8_t channel, float duty_cycle);
PWM_StatusTypeDef Servo_SetAngle(uint8_t channel, float angle);
PWM_StatusTypeDef Servo_SetAngleWithLimit(uint8_t channel, float angle, float min_angle, float max_angle);
```

## 使用方法

### 1. 包含头文件

```c
#include "PWM.h"
```

### 2. 初始化PWM

```c
PWM_Init();
```

### 3. 控制舵机

```c
// 使用兼容接口
Servo_SetAngle1(90.0f);  // 设置舵机1到90度
Servo_SetAngle2(45.0f);  // 设置舵机2到45度

// 使用增强接口（带错误检查）
if (Servo_SetAngle(PWM_CHANNEL_1, 90.0f) != PWM_OK) {
    // 错误处理
}

// 使用带限位保护的接口
if (Servo_SetAngleWithLimit(PWM_CHANNEL_1, 120.0f, 0.0f, 180.0f) != PWM_OK) {
    // 错误处理
}
```

### 4. 设置PWM占空比

```c
// 设置50%占空比
if (PWM_SetDutyCycle(PWM_CHANNEL_1, 50.0f) != PWM_OK) {
    // 错误处理
}
```

## 配置参数

```c
#define PWM_PERIOD_VALUE    1999    // PWM周期值
#define PWM_PRESCALER_VALUE 719     // 预分频值
#define SERVO_MIN_PULSE     50      // 舵机最小脉宽 (0.5ms)
#define SERVO_MAX_PULSE     250     // 舵机最大脉宽 (2.5ms)
#define SERVO_CENTER_PULSE  150     // 舵机中位脉宽 (1.5ms)
#define PWM_CHANNEL_1       1       // PWM通道1
#define PWM_CHANNEL_2       2       // PWM通道2
```

## 错误代码

```c
typedef enum {
    PWM_OK = 0,                      // 操作成功
    PWM_ERROR_INVALID_CHANNEL,       // 无效通道
    PWM_ERROR_INVALID_PARAMETER      // 无效参数
} PWM_StatusTypeDef;
```

## 硬件连接

- PWM通道1：PB6 (TIM4_CH1)
- PWM通道2：PB7 (TIM4_CH2)

## 注意事项

1. 在使用PWM功能前，必须先调用 `PWM_Init()` 进行初始化
2. 舵机角度范围为 0-180 度
3. PWM占空比范围为 0-100%
4. 建议使用带错误检查的增强接口以提高代码健壮性

## 编译

项目使用CMake构建系统，支持ARM Cortex-M3交叉编译：

```bash
# 配置项目
cmake --preset Debug

# 编译项目
cmake --build --preset Debug
```

编译成功后会生成 `bbox.elf` 可执行文件。