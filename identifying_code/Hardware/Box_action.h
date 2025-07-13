#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "Key.H"
#include "stdlib.h"
#include "PWM.h"
#ifndef	 __BOX_ACTION_H
#define  __BOX_ACTION_H

/*设置开合PWM值*/
#define  LID_OPEN_VALUE		   110
#define  LID_NO_OPEN_VALUE	   132
#define  LID_CLOSE_VALUE	   180
#define  PUSH_OPEN_VALUE	   175

#define  PUSH_CLOSE_VALUE	   40
#define  LID_NO_CLOSE_VALUE    145
#define  PUSH_NO_OPEN_VALUE	   120
#define  PUSH_FAKE_OPEN_VALUE  120
                                                                                                  
#define  F_LID				0
#define  F_PUSH				1

#define  OPEN	 			0
#define  CLOSE				1
#define  PUSH  				2
#define  FOLD  				3
#define  NO_CLOSE			4
#define  NO_PUSH			5
#define  NO_OPEN			6
#define  NIL				8

#define  LID_OPEN    		Servo_SetAngle2(LID_OPEN_VALUE)
#define  LID_CLOSE  		Servo_SetAngle2(LID_CLOSE_VALUE)
#define  PUSH_OPEN	 		Servo_SetAngle1(PUSH_OPEN_VALUE)
#define  PUSH_CLOSE	 		Servo_SetAngle1(PUSH_CLOSE_VALUE)

#define	 LID_NO_OPEN 		Servo_SetAngle2(LID_NO_OPEN_VALUE)
#define  LID_NO_CLOSE  		Servo_SetAngle2(LID_NO_CLOSE_VALUE)
#define  PUSH_NO_OPEN	 	Servo_SetAngle1(PUSH_NO_OPEN_VALUE)

#define  Short_time 		800
#define  Long_time 			3000


void Action_Type(uint32_t windup,uint8_t action,uint32_t recovery);
void Action_Slow(uint32_t windup,_Bool part,uint8_t begin,uint8_t final,uint8_t change,uint32_t time,uint32_t recovery);
void Box_action(void);

#endif
