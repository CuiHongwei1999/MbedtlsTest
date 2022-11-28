/*
*********************************************************************************************************
*                                            	 Notes
* Filename: usart.h
* Notes:
* Date: 2020-2
* Author: ´Þºéçâ
*********************************************************************************************************
*/
#ifndef __UART_H
#define __UART_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32l4xx.h"
#include "ram_mgmt.h"

/*
*********************************************************************************************************
*                                              DEFINATIONS
*********************************************************************************************************
*/
#define USE_RX_DMA			1
#define UART_OK     		0
#define UART_ERR    		1
#define RX_BUF_SIZE 		1024
#define ASYNC_RX_BUF_SIZE	1024
#define UNREAD_MSG			1
#define NO_NEW_MSG			0
#define UART_SRAM			SRAM1

typedef struct {
	volatile unsigned char transCplt;
	volatile unsigned char recvCplt;
	volatile unsigned short RxLen;
	volatile unsigned char buffered;
	volatile unsigned char sync;
	unsigned char *RxBuf;
	unsigned char *RxBuf_2;
	UART_HandleTypeDef *uart;
	DMA_HandleTypeDef *dma_rx;
} UART_Sta;


/*
*********************************************************************************************************
*                                            	FUNCTIONS
*********************************************************************************************************
*/
/*
**********************************************************************************
* Name: UART_StaInit(UART_HandleTypeDef uart)	
* Description:
* Parameter(s):
*	UART_HandleTypeDef uart
* Return:
*	0: UART_OK
*	1: UART_ERR
* Date: 2020-2
**********************************************************************************
*/
unsigned int UART_StaInit(UART_Sta *uartSta, UART_HandleTypeDef *uart, DMA_HandleTypeDef *dma_rx);

/*
**********************************************************************************
* Name: myUART_IRQ_Handler(UART_Sta *uartSta)	
* Description:
*	This function should be inserted into function UARTx_IRQ_Handler
*	in stm32h7xx_it.c
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
void myUART_IRQ_Handler(UART_Sta *uartSta);

/*
**********************************************************************************
* Name: myUART_IRQ_Sync_Handler(UART_Sta *uartSta)	
* Description:
*	This function should be inserted into function UARTx_IRQ_Handler
*	in stm32h7xx_it.c
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
void myUART_IRQ_Sync_Handler(UART_Sta *uartSta);

/*
**********************************************************************************
* Name: UART_SendData(UART_HandleTypeDef uart, char *buf, unsigned char free_buf)	
* Description:
* Parameter(s):
*	UART_HandleTypeDef uart
*	char *buf
* Return:
*	0: UART_OK
*	1: UART_ERR
* Date: 2020-2
**********************************************************************************
*/
unsigned int UART_SendData(UART_Sta *uartSta, char *text, unsigned char free_buf);

/*
**********************************************************************************
* Name: UART_SendNumData(UART_HandleTypeDef uart, char *buf, unsigned int num, unsigned char free_buf)	
* Description:
* Parameter(s):
*	UART_HandleTypeDef uart
*	char *buf
*	unsigned int num
* Return:
*	0: UART_OK
*	1: UART_ERR
* Date: 2020-2
**********************************************************************************
*/
unsigned int UART_SendNumData(UART_Sta *uartSta, char *text, unsigned int num, unsigned char free_buf);

/*
**********************************************************************************
* Name: UART_ContGetData
* Description:
* Parameter(s):
* Return:
*	NULL: memory error
*	OTHER: Pointer to the received data
* Date: 2020-2
**********************************************************************************
*/
char* UART_ContGetData(UART_Sta *uartSta, unsigned int continuous_rev_time);

/*
**********************************************************************************
* Name: 	
* Description:
* Parameter(s):
* Return:
* 	NULL: Time out or memory error
*	OTHER: Pointer to the received data
* Date: 2020-2
**********************************************************************************
*/
char* UART_WaitOnRevData(UART_Sta *uartSta, unsigned int timeout);

/*
**********************************************************************************
* Name: UART_WaitOnRevData 	
* Description:
* Parameter(s):
* Return:
* 	NULL: Time out or memory error
*	OTHER: Pointer to the received data
* Date: 2020-2
**********************************************************************************
*/
char* UART_WaitOnRevNumData(UART_Sta *uartSta, unsigned int num, unsigned int timeout);

#endif
