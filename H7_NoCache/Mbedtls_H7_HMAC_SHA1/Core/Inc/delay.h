#ifndef __DELAY_H
#define __DELAY_H
/*
*********************************************************************************************************
*                                          FUNCTION DECLARATION
*********************************************************************************************************
*/

#include "stm32h7xx_hal.h"

extern TIM_HandleTypeDef htim12;	//defined in main.c
#define TIM htim12 				    //edit this to change Timer


/*
*********************************************************************************************************
*                                          FUNCTION DECLARATION
*********************************************************************************************************
*/
void delay_us(unsigned int nus);
void delay_ms(unsigned int nms);


#endif
