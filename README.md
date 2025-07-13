# 无聊盒子项目 (Boring Box Project)

## 项目概述

这是一个基于STM32F103的无聊盒子项目，当用户按下按钮时，盒子会自动打开盖子并关闭按钮开关，实现一个有趣的互动装置。

## 硬件配置

- **微控制器**: STM32F103C8T6
- **按键**: PA2 (输入，内部上拉)
- **舵机1**: PB6 (TIM4_CH1) - 控制盖子开关
- **舵机2**: PB7 (TIM4_CH2) - 控制按钮开关
- **定时器**: TIM2 用于按键扫描，TIM4 用于PWM输出

## 项目架构

### 模块化设计

项目采用模块化设计，主要包含以下模块：

#### 1. PWM模块 (`PWM.h` / `PWM.c`)
- **功能**: 提供PWM信号生成和舵机控制
- **兼容接口**: 
  - `PWM_Init()` - PWM初始化
  - `PWM_SetCompare1(uint16_t compare)` - 设置通道1比较值
  - `PWM_SetCompare2(uint16_t compare)` - 设置通道2比较值
  - `Servo_SetAngle1(float angle)` - 设置舵机1角度
  - `Servo_SetAngle2(float angle)` - 设置舵机2角度
- **增强接口**:
  - `PWM_SetDutyCycle(uint32_t channel, float duty_cycle)` - 设置占空比
  - `Servo_SetAngle(uint32_t channel, float angle)` - 通用舵机角度设置
  - `Servo_SetAngleWithLimit(uint32_t channel, float angle, float min_angle, float max_angle)` - 带限制的角度设置

#### 2. Key模块 (`Key.h` / `Key.c`)
- **功能**: 按键检测和去抖处理
- **主要函数**:
  - `Key_Init()` - 按键模块初始化
  - `Key()` - 获取按键状态并清零
  - `Get_Key()` - 获取当前按键电平
  - `Loop_Key()` - 按键扫描循环
- **特性**: 使用TIM2定时器中断进行25ms周期的按键扫描和去抖

#### 3. Box_action模块 (`Box_action.h` / `Box_action.c`)
- **功能**: 实现盒子的各种动作逻辑
- **主要函数**:
  - `Box_action()` - 主动作函数，随机执行24种不同的动作模式
  - `Action_Type(uint16_t time, uint8_t servo, uint16_t pwm_value)` - 简单动作
  - `Action_Slow(uint16_t time, uint8_t servo, uint16_t start, uint16_t end, uint8_t step, uint16_t delay, uint16_t wait)` - 缓慢动作
  - `Delay_ms(uint16_t time)` - 延时函数

### 动作模式

项目包含24种不同的动作模式，每次按键触发时随机选择一种：
- 快速开关动作
- 缓慢渐变动作
- 窥视动作
- 组合动作序列

## 编译和构建

### 环境要求
- CMake 3.22+
- ARM GCC 工具链
- Ninja 构建系统

### 构建步骤

1. **配置项目**:
   ```bash
   cmake --preset Debug
   ```

2. **编译项目**:
   ```bash
   cmake --build --preset Debug
   ```

3. **输出文件**: `build/Debug/bbox.elf`

### 内存使用
- **RAM**: 2168 B / 20 KB (10.59%)
- **FLASH**: 16684 B / 64 KB (25.46%)

## 使用方法

1. **硬件连接**:
   - 将舵机1连接到PB6
   - 将舵机2连接到PB7
   - 将按键连接到PA2和GND

2. **程序烧录**:
   - 使用ST-Link或其他烧录工具将`bbox.elf`烧录到STM32F103

3. **操作**:
   - 按下按键，盒子将执行随机动作
   - 每次按键都会触发不同的动作序列

## 代码特性

- **模块化设计**: 各功能模块独立，便于维护和扩展
- **兼容性**: PWM模块提供与开源代码兼容的接口
- **可扩展性**: 支持添加新的动作模式和功能
- **稳定性**: 使用定时器中断进行按键去抖，确保可靠的按键检测
- **随机性**: 每次触发随机选择动作，增加趣味性

## 文件结构

```
Core/
├── Inc/
│   ├── main.h              # 主头文件
│   ├── PWM.h               # PWM模块头文件
│   ├── Key.h               # 按键模块头文件
│   └── Box_action.h        # 动作模块头文件
└── Src/
    ├── main.c              # 主程序
    ├── PWM.c               # PWM模块实现
    ├── Key.c               # 按键模块实现
    └── Box_action.c        # 动作模块实现
```

## 注意事项

1. **舵机电源**: 确保舵机有足够的电源供应
2. **PWM频率**: 舵机PWM频率设置为50Hz (20ms周期)
3. **角度范围**: 舵机角度范围通常为0-180度
4. **按键去抖**: 系统已实现25ms的按键去抖处理
5. **随机种子**: 可根据需要修改随机数生成方式

## 扩展建议

- 添加LED指示灯显示状态
- 增加蜂鸣器音效
- 支持多个按键输入
- 添加无线控制功能
- 实现动作序列的自定义配置