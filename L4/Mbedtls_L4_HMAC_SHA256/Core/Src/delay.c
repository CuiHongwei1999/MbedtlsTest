/*
**************************************************************************
*   DECLARATION
*   (1)If the value of ARR (auto-reload register) is 0, the Timer will not 
*   work. This bug should be extra careful.
**************************************************************************
*/

#include "delay.h"

#define limit (0xFFFF - 5)



/*
**************************************************************************
*	Function Name: delay_us()
*	Formal Parameter: nus
*	
*	Notes:
*(1)Use Timer12
*	The maximum delay time is no more than 65531us
*	Since 
*		2^16 - 5 = 65531
*(2)define limit as 0xFFFF - 5 to prevent from coming into interruption.
**************************************************************************
*/
void delay_us(unsigned int nus)
{	
	unsigned int differ = limit - nus;
	
	HAL_TIM_Base_Start(&TIM);
	
	__HAL_TIM_SetCounter(&TIM, differ);

	while (differ < limit)
	{
		differ = __HAL_TIM_GetCounter(&TIM);
	}
}


/*
**************************************************************************
*	Function Name: delay_ms()
*	Formal Parameter: nms
*	
*	Notes:
*	none
**************************************************************************
*/
void delay_ms(unsigned int nms)
{
	for (unsigned int i = 0; i < nms; i++) {
		delay_us(1000);
	}
}
	
	


