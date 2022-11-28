#ifndef __DELAY_H
#define __DELAY_H
/*
*********************************************************************************************************
*                                          FUNCTION DECLARATION
*********************************************************************************************************
*/

#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef htim17;	//defined in main.c
#define TIM htim17 				    //edit this to change Timer


/*
*********************************************************************************************************
*                                          FUNCTION DECLARATION
*********************************************************************************************************
*/
void delay_us(unsigned int nus);
void delay_ms(unsigned int nms);


#endif
