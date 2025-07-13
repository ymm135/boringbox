#include "stm32f10x.h"                  // Device header
uint8_t Key_KeyNumber;
						//**定时器扫描按键**//
						
void GPIO_KeyInit(void)	//初始化GPIO								
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void Timer_Init(void)//初始化定时器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_Period=250-1;
	TimeBaseInitStructure.TIM_Prescaler=7200-1;
	TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

uint8_t Key(void)//获取按键值
{
	uint8_t	Temp;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;
	return Temp;
}

uint8_t Get_Key(void)//获取电平变化
{	
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		KeyNum=1;
	}
	return KeyNum;
}

void Loop_Key(void)//扫描按键
{	
	static uint8_t LastKey=0,NowKey=0;
	LastKey=NowKey;
	NowKey=Get_Key();
	if(LastKey==0 && NowKey==1)
	{
		Key_KeyNumber=1;
	}
}

void Key_Init(void)//按键初始化
{	
	Timer_Init();
	GPIO_KeyInit();
}
extern uint8_t KeyNum;
extern uint8_t mode;
void TIM2_IRQHandler(void)//定时器扫描（30ms）
{
	static uint8_t scan_time=0;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==1)
	{	scan_time++;
		if(scan_time>33)
		{
			scan_time=0;
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0 && !KeyNum)
			{
				KeyNum=1;
			}
		}
		Loop_Key();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
