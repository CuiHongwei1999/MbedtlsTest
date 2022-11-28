/*
*********************************************************************************************************
*                                            	 Notes
* Filename: 
* Notes:
* Date: 2020-2
* Author: ´Þºéçâ
*********************************************************************************************************
*/
#ifndef __ATOI_ITOA_H
#define __ATOI_ITOA_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             DEFINATIONS
*********************************************************************************************************
*/	

/*
*********************************************************************************************************
*                                              FUNCTIONS
*********************************************************************************************************
*/
/*
**********************************************************************************
* Name: my_atoi(char *str)	
* Description:
*	transform number char* into integer
*	i.e. "  -123456" --> -123456
*	auto remove leading spaces and "+"
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
int my_atoi(char *str);

/*
**********************************************************************************
* Name: my_atoi_ll(char *str)	
* Description:
*	transform number char* into long long
*	i.e. "  -123456" --> -123456
*	auto remove leading spaces and "+"
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
long long my_atoi_ll(char *str);

/*
**********************************************************************************
* Name: my_strlen(char *str)	
* Description:
*	count the string length expect '/0'
* Parameter(s):
*	char *str
* Return:
* Date: 2020-2
**********************************************************************************
*/
unsigned int my_strlen(char *str);

/*
**********************************************************************************
* Name: reverseStr(char *str)	
* Description:
*	reverse char array without any type limination
*	(relative to reverseeCharArray()).
*	noticed that this fun will use malloc,
*	and remember to free the ram space when var out of date.
* Parameter(s):
*	char *str
* Return:
* Date: 2020-2
**********************************************************************************
*/
char* reverseStr(char *str);

/*
**********************************************************************************
* Name: my_itoa(int num)	
* Description:
*	transform an integer to a string. noticed that this fun will use malloc,
*	and remember to free the ram space when var out of date.
* Parameter(s):
*	int num
* Return:
* Date: 2020-2
**********************************************************************************
*/
char* my_itoa(int num);

/*
**********************************************************************************
* Name: my_strcpy(char *str)	
* Description:
* Parameter(s):
* Return:
*	char*: OK
*	NULL : memory error
* Date: 2020-2
**********************************************************************************
*/
char *my_strcpy(char *str);

/*
**********************************************************************************
* Name: float2string
* Description: 
*	transform the float into string and return the string address
*	Notes:
*		the output format is ?xxx.xx.
*		the max input float is 999.99 and the minimum is -999.99
* Parameter(s):
*	float num
* Return:
* Date: 2019-7
**********************************************************************************
*/
char* float2string(float num);

#endif
