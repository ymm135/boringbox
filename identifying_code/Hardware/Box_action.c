#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"                
#include "Box_action.h"

uint8_t Cnt=0;
uint8_t mode=0;
uint8_t random_time;
_Bool Action;
const uint8_t mode_num=24;
/**
 * @brief 简易动作（单纯开关）：用于推杆、盖子简单的开合动作
 * @param windup   释放前的等待时间
 * @param action   释放的动作类型
 * @param recovery 释放后的等待时间
 * @return 	无
 */
void Action_Type(uint32_t windup,uint8_t action,uint32_t recovery)//前摇，类型，后摇
{	
	Delay_ms(windup);
	switch(action)
	{
		case 0:LID_OPEN;break;
		case 1:LID_CLOSE;break;
		case 2:PUSH_OPEN;break;
		case 3:PUSH_CLOSE;break;
		case 4:LID_NO_CLOSE;break;
		case 5:PUSH_NO_OPEN;break;
		case 6:LID_NO_OPEN;break;
		default:break;
	}
	Delay_ms(recovery);
}

void Action_Slow(uint32_t windup,_Bool part,uint8_t begin,uint8_t final,uint8_t change,uint32_t time,uint32_t recovery)
{
	Delay_ms(windup);
	if(part)//PUSH
	{
		if(begin<final)
		{
			for(uint8_t i=begin;i<final;i+=change)
			{
				Servo_SetAngle1(i);
				Delay_ms(time);
			}		
		}
		else
		{
			for(uint8_t i=begin;i>final;i-=change)
			{
				Servo_SetAngle1(i);
				Delay_ms(time);
			}	
		}
	}
	else//LIP
	{
		if(begin<final)
		{
			for(uint8_t i=begin;i<final;i+=change)
			{
				Servo_SetAngle2(i);
				Delay_ms(time);
			}		
		}
		else
		{
			for(uint8_t i=begin;i>final;i-=change)
			{
				Servo_SetAngle2(i);
				Delay_ms(time);
			}	
		}		
	}
	Delay_ms(recovery);
}


void Box_action(void)
{	Action=1;
	TIM_Cmd(TIM4, ENABLE);
	switch(mode)
	{
		case 0://开盖、推杆、收杆、关盖
		Action_Type(Short_time,OPEN,900);
		Action_Type(NULL,PUSH,700);
		Action_Type(NULL,FOLD,500);
		Action_Type(NULL,CLOSE,100);
		break;	
		
		case 1://开盖、推杆、收杆、关盖
		Action_Type(Short_time,OPEN,500);
		Action_Type(NULL,PUSH,400);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,80);
		break;
		
		case 2:
		//缓慢开盖，推杆移动到钮子前，收杆，推杆，收杆，关盖
		Action_Slow(Short_time,F_LID,LID_CLOSE_VALUE,LID_OPEN_VALUE,2,50,1000);
		Action_Slow(NULL,F_PUSH,PUSH_CLOSE_VALUE,PUSH_FAKE_OPEN_VALUE,2,10,1000);
		Action_Type(NULL,FOLD,50);
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,220);
		Action_Type(NULL,CLOSE,300);	
		break;
			
		case 3://缓慢开盖，关盖，开盖，推杆，收杆，关盖
		Action_Slow(Long_time,F_LID,LID_CLOSE_VALUE,LID_OPEN_VALUE,2,50,1600);
		Action_Slow(NULL,F_LID,LID_OPEN_VALUE,LID_CLOSE_VALUE,2,50,2000);
		Action_Type(NULL,OPEN,250);
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,220);
		Action_Type(NULL,CLOSE,50);
		break;
		
		case 4://开盖、推杆、收杆、关盖
		Action_Type(Short_time,OPEN,100);
		Action_Type(NULL,PUSH,280);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,50);	
		break;
		
		case 5://开盖，连续推杆，收杆，关盖，开盖，推杆，收杆，关盖
		Action_Type(Short_time,OPEN,500);
		Action_Slow(NULL,F_PUSH,PUSH_CLOSE_VALUE,PUSH_FAKE_OPEN_VALUE,2,10,1600);
		for(uint8_t i=0;i<8;i++)
		{		
			Action_Type(NULL,FOLD,80);
			Action_Type(NULL,PUSH,80);
		}
		Action_Type(500,NO_CLOSE,100);
		Action_Type(2000,OPEN,180);
		Action_Type(NULL,FOLD,100);
		Action_Type(NULL,CLOSE,100);
		break;
		
		case 6://开盖，推杆，扑打盖，收杆，关盖
		Action_Type(Short_time,OPEN,500);
		Action_Type(NULL,PUSH,500);
		for(uint8_t i=0;i<12;i++)
		{
			Action_Type(NULL,NO_CLOSE,80);
			Action_Type(NULL,OPEN,100);
		}
		Action_Type(300,FOLD,600);
		Action_Type(NULL,CLOSE,50);
		break;
		
		case 7://开盖，关盖，开盖，推杆，收杆，关盖
		Action_Type(Short_time,OPEN,500);
		Action_Type(NULL,CLOSE,300);
		Action_Type(NULL,OPEN,1200);
		Action_Type(NULL,PUSH,400);
		Action_Type(NULL,FOLD,200);		
		Action_Type(NULL,CLOSE,300);
		break;
		
		case 8://开盖，颤抖盖，连续推杆，收杆，关盖
		Action_Slow(Long_time,F_LID,LID_CLOSE_VALUE,LID_OPEN_VALUE,2,30,500);
		for(uint8_t i=0;i<18;i++)
		{
		Action_Type(NULL,NO_CLOSE,40);	
		Action_Type(NULL,OPEN,40);				
		}Action_Type(NULL,PUSH,100);	
		for(uint8_t i=0;i<10;i++)
		{
		Action_Type(NULL,PUSH,150);	
		Action_Type(NULL,FOLD,50);					
		}
		Action_Type(NULL,CLOSE,40);
		break;		
		
		case 9://开盖，关盖
		Action_Type(Short_time,OPEN,1500);
		Action_Type(NULL,CLOSE,200);
		break;
		
		case 10://开盖，关盖，开盖，推杆，收杆，关盖
		Action_Type(Short_time,OPEN,1500);
		Action_Type(NULL,CLOSE,2000);
		Action_Type(NULL,OPEN,200);
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,300);
		break;
		
		case 11://缓慢开盖，快速推杆，收杆，关盖
		Action_Slow(Short_time,F_LID,LID_CLOSE_VALUE,LID_NO_CLOSE_VALUE,2,40,1500);
		Action_Slow(NULL,F_LID,LID_NO_CLOSE_VALUE,LID_CLOSE_VALUE-40,2,50,800);
		Action_Type(NULL,OPEN,30);
		Action_Type(NULL,PUSH,500);
		Action_Type(NULL,FOLD,80);
		Action_Type(NULL,CLOSE,20);
		break;
		
		case 12://开盖，假推杆，收回，推杆，收回，关盖
		Action_Type(Short_time,OPEN,1500);
		Action_Type(NULL,NO_PUSH,250);
		Action_Type(NULL,FOLD,200);
		Action_Type(Short_time,OPEN,800);
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,300);		
		break;
		
		case 13://开盖，连续假推杆，推杆，收回，关盖
		Action_Type(Short_time,OPEN,1500);
		for(uint8_t i=0;i<5;i++)
		{
			Action_Type(NULL,NO_PUSH,250);
			Action_Type(NULL,FOLD,100);		
		}
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,300);		
		break;
		
		case 14://半开盖，全开盖，推杆，收回，关盖
		Action_Type(Short_time,NO_OPEN,800);
		Action_Type(NULL,OPEN,100);
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,300);		
		break;
		
		case 15://（半开盖，全开盖）循环5次，推杆，收回，关盖
		Action_Slow(Short_time,F_LID,LID_CLOSE_VALUE,LID_OPEN_VALUE,2,40,400);
		for(uint8_t i=0;i<5;i++)
		{
		Action_Type(1000,NO_OPEN,100);
		Action_Type(NULL,OPEN,50);	
		}

		Action_Type(NULL,PUSH,280);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,200);
		break;
		
		case 16://开盖，立马推杆，缓慢收回，关盖
		Action_Type(Short_time,OPEN,20);
		Action_Type(NULL,PUSH,2000);
		Action_Slow(Short_time,F_PUSH,PUSH_OPEN_VALUE,PUSH_CLOSE_VALUE,2,80,400);
		Action_Type(NULL,CLOSE,200);
		break;
		
		case 17://开盖，关盖（循环3次）延时开盖，推杆，收回，关盖
		for(uint8_t i=0;i<3;i++)
		{
			Action_Type(800,OPEN,600);
			Action_Type(50,CLOSE,200);
		}
		Action_Type(2000,OPEN,30);
		Action_Type(300,PUSH,260);
		Action_Type(30,FOLD,200);
		Action_Type(30,CLOSE,30);
		break;
		
		case 18://开盖，缓慢推到开关前，加速推，缓慢回，关盖
		Action_Type(Short_time,OPEN,400);
		Action_Slow(50,F_PUSH,PUSH_CLOSE_VALUE,PUSH_FAKE_OPEN_VALUE-20,2,80,40);
		Action_Type(NULL,PUSH,260);
		Action_Slow(50,F_PUSH,PUSH_OPEN_VALUE,PUSH_CLOSE_VALUE+20,2,80,40);
		Action_Type(50,CLOSE,50);
		break;
		
		case 19://开盖，推杆延时，收回，关盖
		Action_Type(Short_time,OPEN,1000);
		Action_Type(NULL,PUSH,2000);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,200);
		break;
		
		case 20://开盖，假推，收回（循环3次），推杆，收回，关盖
		Action_Type(Short_time,OPEN,400);
		for(uint8_t i=0;i<2;i++)
		{
			Action_Type(NULL,NO_PUSH,240);
			Action_Type(NULL,FOLD,800);
		}
		Action_Type(NULL,PUSH,260);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,200);
		break;
		
		case 21://开盖，假推，收回（循环随机<7次），推杆，收回，关盖
		Action_Type(Short_time,OPEN,400);
		for(uint8_t i=0;i<random_time%6+2;i++)
		{
			Action_Type(NULL,NO_PUSH,240);
			Action_Type(NULL,FOLD,800);
		}
		Action_Type(NULL,PUSH,250);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,200);
		break;
		
		case 22://开盖，连续推杆，收杆，关盖，开盖检查开关是否又被按下来决定是否推杆
		Action_Type(Short_time,OPEN,200);
		Action_Type(NULL,PUSH,250);
		for(uint8_t i=0;i<6;i++)
		{
			Action_Type(NULL,FOLD,250);
			Action_Type(NULL,PUSH,260);
		}
		Action_Type(NULL,PUSH,1000);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,3000);
		Action_Type(200,OPEN,2000);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
		{
			Action_Type(NULL,PUSH,260);
			Action_Type(NULL,FOLD,200);
		}		
		Action_Type(200,CLOSE,200);
		break;

		case 23://开盖，连续缓慢假推杆，推杆，收杆，关盖
		Action_Type(Short_time,OPEN,400);
		for(uint8_t i=0;i<3;i++)
		{
		Action_Slow(50,F_PUSH,PUSH_CLOSE_VALUE,PUSH_FAKE_OPEN_VALUE,2,30,40);
		Action_Type(NULL,FOLD,80);		
		}
		Action_Type(NULL,PUSH,300);
		Action_Type(NULL,FOLD,200);
		Action_Type(50,CLOSE,50);		
		break;
		
		default ://开盖，推杆，收杆，关盖
		Action_Type(Short_time,OPEN,100);
		Action_Type(NULL,PUSH,280);
		Action_Type(NULL,FOLD,200);
		Action_Type(NULL,CLOSE,50);
		break;
	}
	Action=0;
	Delay_ms(300);
	TIM_Cmd(TIM4, DISABLE);
}


