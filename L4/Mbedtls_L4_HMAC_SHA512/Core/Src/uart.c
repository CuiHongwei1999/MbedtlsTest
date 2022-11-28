/*
*********************************************************************************************************
*                                            	 Notes
* Filename: usart.c
* Notes:
* Date: 2020-2
* Author: 崔洪玮
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "uart.h"
#include "ram_mgmt.h"
#include "my_list.h"
#include "atoi_itoa.h"
#include "delay.h"
#include <string.h>

/*
*********************************************************************************************************
*                                             DEFINATIONS
*********************************************************************************************************
*/
#define CONTINUOUS_REV_TIME	 		1
#define MAX_UART_INTERRUPT_DELAY	0x1FFFF

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
static node *uartList = NULL;
extern UART_HandleTypeDef huart2;

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
unsigned int UART_StaInit(UART_Sta *uartSta, UART_HandleTypeDef *uart, DMA_HandleTypeDef *dma_rx) {
	HAL_StatusTypeDef sta;
	
	uartSta->recvCplt = 0;
	uartSta->RxLen = 0;
	uartSta->RxBuf = my_malloc(UART_SRAM, RX_BUF_SIZE);
	if (uartSta->RxBuf) {
		my_memclear((char *)uartSta->RxBuf, RX_BUF_SIZE);
	}
	
	uartSta->transCplt = NO_NEW_MSG;
	
	uartSta->uart = uart;
	uartSta->dma_rx = dma_rx;
	
	if (AppendList(uartSta, &uartList) == NULL) {
		return UART_ERR;
	}
	
#if USE_RX_DMA
	__HAL_UART_CLEAR_IDLEFLAG(uartSta->uart);
	sta = HAL_UART_Receive_DMA(uart, uartSta->RxBuf, RX_BUF_SIZE);
	__HAL_UART_ENABLE_IT(uart, UART_IT_IDLE);	// ENABLE IDLE frame detection
	
	return (sta != HAL_OK);
#else
	HAL_UART_Receive_IT(uartSta->uart, uartSta->RxBuf, 1);
	return 0;
#endif
}

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
void myUART_IRQ_Sync_Handler(UART_Sta *uartSta) {

	if (uartSta->sync == 0) {
		return;
	}
	if (uartSta->uart == NULL) {
		return;
	}
#if USE_RX_DMA	
	HAL_StatusTypeDef sta = HAL_OK;	// HAL_OK = 0x00
	if (__HAL_UART_GET_FLAG(uartSta->uart, UART_FLAG_IDLE) != RESET) {
		__HAL_UART_CLEAR_IDLEFLAG(uartSta->uart);
		sta += HAL_UART_DMAStop(uartSta->uart);
		sta += HAL_UART_Receive_DMA(uartSta->uart, uartSta->RxBuf, 1024);
		if (sta == HAL_OK) {
			uartSta->RxLen = my_strlen((char *)uartSta->RxBuf);
			my_memcpy(uartSta->RxBuf_2 + uartSta->buffered, uartSta->RxBuf, uartSta->RxLen);
			my_memclear((char *)uartSta->RxBuf, uartSta->RxLen);
			uartSta->buffered += uartSta->RxLen;
		}
	}
#else
	// uartSta->recvCplt = 1;
	while(HAL_UART_Receive_IT(uartSta->uart, uartSta->RxBuf, RX_BUF_SIZE) != HAL_OK);
#endif
}


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
void myUART_IRQ_Handler(UART_Sta *uartSta) {

	if (uartSta->sync == 1) {
		return;
	}
	if (uartSta->uart == NULL) {
		return;
	}
#if USE_RX_DMA	
	HAL_StatusTypeDef sta = HAL_OK;	// HAL_OK = 0x00
	if (__HAL_UART_GET_FLAG(uartSta->uart, UART_FLAG_IDLE) != RESET) {
		__HAL_UART_CLEAR_IDLEFLAG(uartSta->uart);
		sta += HAL_UART_DMAStop(uartSta->uart);
		sta += HAL_UART_Receive_DMA(uartSta->uart, uartSta->RxBuf, RX_BUF_SIZE);
		uartSta->recvCplt = (sta == HAL_OK);
	}
#else
	// uartSta->recvCplt = 1;
	while(HAL_UART_Receive_IT(uartSta->uart, uartSta->RxBuf, RX_BUF_SIZE) != HAL_OK);
#endif
}

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
unsigned int UART_SendData(UART_Sta *uartSta, char *text, unsigned char free_buf) {
	HAL_StatusTypeDef sta = HAL_OK;
	
	sta = HAL_UART_Transmit_DMA(uartSta->uart, (unsigned char*)text, strlen(text));
	uartSta->transCplt = (sta == HAL_OK);
	
	if (free_buf != 0) {
		my_free(UART_SRAM, text);
	}
	
	delay_ms(5);
	return (sta != HAL_OK);
}

/*
**********************************************************************************
* Name: UART_SendNumData(UART_HandleTypeDef uart, char *buf, unsigned int num)	
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
unsigned int UART_SendNumData(UART_Sta *uartSta, char *text, unsigned int num, unsigned char free_buf) {
	HAL_StatusTypeDef sta = HAL_OK;
	
	sta = HAL_UART_Transmit_DMA(uartSta->uart, (unsigned char*)text, num);
	uartSta->transCplt = (sta == HAL_OK);
	
	if (free_buf != 0) {
		my_free(UART_SRAM, text);
	}
	
	delay_ms(5);
	return (sta != HAL_OK);
}

/*
**********************************************************************************
* Name: GetContent(node *position)	
* Description:
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
static void* GetContent(node *position) {
	return (((UART_Sta*)position->content)->uart);
}

/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	node *uartNode = SearchList(huart, uartList, GetContent);
	if (uartNode != NULL) {
		((UART_Sta*)uartNode->content)->transCplt = 1;
	}
}

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
char* UART_ContGetData(UART_Sta *uartSta, unsigned int continuous_rev_time) {
	char *data = NULL;
	static unsigned int rev_data_len = 0;
	static unsigned int continuous_rev_tick_start = 0;
	
	if (continuous_rev_tick_start == 0) {
		data = NULL;
		continuous_rev_tick_start = HAL_GetTick();
	}
	while (continuous_rev_time > (HAL_GetTick() - continuous_rev_tick_start)) {
		if (uartSta->recvCplt == 1) {
			// 计算接收到的数据长度
			uartSta->RxLen = my_strlen((char *)uartSta->RxBuf);
			// 为新数据和之前的数据重新分配空间
			if (uartSta->RxLen != 0) {
				data = my_realloc(UART_SRAM, data, rev_data_len + uartSta->RxLen);
				// 如果空间成功分配
				if (data != NULL) {
					// 将buf里的数据拷贝到原有数据后面
					my_memcpy((char*)(data + rev_data_len), uartSta->RxBuf , uartSta->RxLen);
					rev_data_len += uartSta->RxLen;
					// 清空buf
					my_memclear((char *)uartSta->RxBuf, uartSta->RxLen);
					uartSta->RxLen = 0;
					uartSta->recvCplt = 0;
				} else {
					// 空间分配失败
					return NULL;
				}
			}
		}			
	}
	// 如果已经到了截止时间
	*(data + rev_data_len) = '\0';
	continuous_rev_tick_start = 0;
	rev_data_len = 0;
	return data;
}

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
char* UART_WaitOnRevData(UART_Sta *uartSta, unsigned int timeout) {
	unsigned int tickstart = HAL_GetTick();
	char *data = NULL;
	
	while (uartSta->recvCplt == 0) {
		if ((HAL_GetTick() - tickstart) > timeout) {
			return NULL;
		}
	}
	
	uartSta->RxLen = my_strlen((char *)uartSta->RxBuf);
	data = my_malloc(UART_SRAM, uartSta->RxLen);
	if (data != NULL) {
		my_memclear(data, uartSta->RxLen);
		my_memcpy(data, uartSta->RxBuf, uartSta->RxLen);
		my_memclear((char *)uartSta->RxBuf, uartSta->RxLen);
		uartSta->RxLen = 0;
		uartSta->recvCplt = 0;
	}
	return (data);
}

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
char* UART_WaitOnRevNumData(UART_Sta *uartSta, unsigned int num, unsigned int timeout) {
	unsigned int tickstart = HAL_GetTick();
	char *data = NULL;
	
	while (uartSta->recvCplt == 0) {
		if ((HAL_GetTick() - tickstart) > timeout) {
			return NULL;
		}
	}
	
	uartSta->RxLen = num;
	data = my_malloc(UART_SRAM, uartSta->RxLen);
	if (data != NULL) {
		my_memclear(data, uartSta->RxLen);
		my_memcpy(data, uartSta->RxBuf, uartSta->RxLen);
		my_memclear((char *)uartSta->RxBuf, uartSta->RxLen);
		uartSta->RxLen = 0;
		uartSta->recvCplt = 0;
	}
	return (data);
}
