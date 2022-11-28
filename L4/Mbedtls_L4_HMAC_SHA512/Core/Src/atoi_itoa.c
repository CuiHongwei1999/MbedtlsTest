/*
*********************************************************************************************************
*                                            	 Notes
* Filename: str_num.c
* Notes: Strings and Numbers convert to each other
* Date: 2020-2
* Author: ´Şºéçâ
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "atoi_itoa.h"
#include "ram_mgmt.h"

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
*********************************************************************************************************
*                                             GLOBAL_VARS
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
int my_atoi(char *str) {
	long result = 0;
	int mul = 1;
	if (str == NULL) {
		return 0;
	} else {
		while(*str == ' ') {
			str++;
		}
		
		if (*str == '-') {
			mul = -1;
			str++;
		} else if(*str == '+'){
			str++;
		}
		
		while (*str >= '0' && *str <= '9') {
			result = result * 10 + (*str - '0');
			if (result > 0xFFFFFFFF) {
				return 0;
			}
			str++;
		}
		
		my_free(ATOI_ITOA_SRAM, str);
		
		return ((int) result * mul);
	}
}

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
long long my_atoi_ll(char *str) {
	long long result = 0;
	int mul = 1;
	if (str == NULL) {
		return 0;
	} else {
		while(*str == ' ') {
			str++;
		}
		
		if (*str == '-') {
			mul = -1;
			str++;
		} else if(*str == '+'){
			str++;
		}
		
		while (*str >= '0' && *str <= '9') {
			result = result * 10 + (*str - '0');
			str++;
		}
		
		my_free(ATOI_ITOA_SRAM, str);
		
		return (result * mul);
	}
}

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
unsigned int my_strlen(char *str) {
    unsigned int counter = 0;
    while(*str) {
        counter++;
        str++;
    }
    return counter;
}

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
char *my_strcpy(char *str) {
	unsigned int length = my_strlen(str) + 1;
	char *strCpy = my_malloc(ATOI_ITOA_SRAM, length);
	
	if (strCpy != NULL) {
		my_memclear(strCpy, length);
		my_memcpy(strCpy, str, length);
		return strCpy;
	} else {
		return NULL;
	}
}

/*
**********************************************************************************
* Name: reverseCharArray(char *str)	
* Description:
*	reverse the character older of string. noticed that the param str must not
*	been "const", you can use char array or malloc ram space directly.
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
char* reverseCharArray(char *str) {
    char temp;
    char *ps = str;	// start
    char *pe = str;	// ending

    while (*pe) {
        pe++;
    }
    pe--;

    while(ps < pe) {
        temp = *ps;
        *ps++ = *pe;
        *pe-- = temp;
    }

    return (str);
}

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
char* reverseStr(char *str) {
    unsigned int strLen = my_strlen(str);
    char *revStr = my_malloc(ATOI_ITOA_SRAM, strLen);
    char *pr = revStr + strLen;
    char *ps = str;
    *pr = '\0';
    pr--;
    while (*ps) {
        *pr-- = *ps++;
    }
	my_free(ATOI_ITOA_SRAM, str);
    return (revStr);
}

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
char* my_itoa(int num) {
    char *str = my_malloc(ATOI_ITOA_SRAM, 80);
    char *n = str;
    unsigned char isNegative = 0;

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    do {
        *n++ = num % 10 + '0';
        num /= 10;
    }while(num > 0);

    if (isNegative) {
        *n++ = '-';
    }

    *n = '\0';
    return (reverseCharArray(str));
}

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
const unsigned char num2char[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};

char* float2string(float num) {
	unsigned char H, D, U, u, d;
	unsigned int temp; 
	
	static char string[7];
	if (num < 0) {
		string[0] = '-';
		num = -num;
	} else {
		string[0] = ' ';
	}
	
	temp = (unsigned int)((num + 0.005f) * 100.0f);	//ËÄÉáÎåÈë
		
	H = temp / 10000;
	D = temp % 10000 / 1000;
	U = temp % 1000 / 100;
	u = temp % 100 / 10;
	d = temp % 10;
	
	if (H != 0) {
		string[1] = num2char[H];
	} else {
		string[1] = ' ';
	}
	
	if (D == 0 && H == 0) {
		string[2] = ' ';
	} else {
		string[2] = num2char[D];
	}
	
	string[3] = num2char[U];
	string[4] = '.';
	string[5] = num2char[u];
	string[6] = num2char[d];
	
	return (string);
}
