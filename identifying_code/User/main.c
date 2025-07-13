/*别烦我盒子V1.1*/
#include "Box_action.h"
extern uint8_t Cnt;
extern uint8_t mode;
extern uint8_t mode_num;
_Bool statu;
uint8_t KeyNum;
int main()
{
	Key_Init();
	PWM_Init();
	Action_Type(500,FOLD,150);
	Action_Type(NULL,CLOSE,80);
	Action_Slow(Long_time,F_LID,LID_CLOSE_VALUE,LID_OPEN_VALUE,2,40,1600);//窥视
	Action_Slow(NULL,F_LID,LID_OPEN_VALUE,LID_CLOSE_VALUE,2,50,500);
	TIM_Cmd(TIM4, DISABLE);
	while(1)
	{	
		KeyNum=Key();
		mode=rand()%mode_num;
		if(KeyNum)Box_action();
	}
}


