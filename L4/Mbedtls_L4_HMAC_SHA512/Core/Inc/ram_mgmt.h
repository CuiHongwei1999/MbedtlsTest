/*
**********************************************************************************************************************
*                                            	       Notes
* Filename: ram_mgmt.h
* Notes:
* Date: 2019
* Author: �޺���
**********************************************************************************************************************
*/
#ifndef __RAM_MGMT_H
#define __RAM_MGMT_H	 

#ifndef NULL
#define NULL 0
#endif

/*
*********************************************************************************************************
*                                             DEFINATIONS
*********************************************************************************************************
*/	
// ���������ڴ��
enum sram {
	SRAM1, 	//  SRAM1
	SRAM2, 	//  SRAM2
};

#define SRAMBANK 		2	// ����֧�ֵ�SRAM����.	
#define MEM_BLOCK_SIZE	64	// ͳһ�����ڴ���С

// mem1�ڴ�����趨.mem1��H7�ڲ���AXI�ڴ�.
#define MEM1_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			60 * 1024  						// total memory size: 96K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	// �ڴ���С

// mem2�ڴ�����趨.mem2���ⲿ��SDRAM�ڴ�
#define MEM2_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			30 * 1024  						// total memory size
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	// �ڴ���С	 


// �ڴ���������
struct _m_mallco_dev
{
	void (*init)(enum sram);						// ��ʼ��
	unsigned short (*perused)(enum sram);			// �ڴ�ʹ����
	unsigned char 	*membase[SRAMBANK];				// �ڴ�� ����SRAMBANK��������ڴ�
	unsigned int 	*memmap[SRAMBANK]; 				// �ڴ����״̬��
	unsigned char  	memrdy[SRAMBANK]; 				// �ڴ�����Ƿ����
};

/*
*********************************************************************************************************
*                                              FUNCTIONS
*********************************************************************************************************
*/
// �û����ú���
void my_free(enum sram memx, void *ptr);  											// �ڴ��ͷ�(�ⲿ����)
void *my_malloc(enum sram memx, unsigned int size);									// �ڴ����(�ⲿ����)
void *my_realloc(enum sram memx, void *ptr, unsigned int size);						// ���·����ڴ�(�ⲿ����)
unsigned short my_mem_perused(enum sram memx) ;										// ����ڴ�ʹ����(��/�ڲ�����) 
void my_mem_init(enum sram memx);													// �ڴ�����ʼ������(��/�ڲ�����)
void my_memset(void *s, unsigned char c, unsigned int count);  						// ��������һ���ڴ��е�ֵ
void my_memcpy(void *des, void *src, unsigned int n);								// �ڴ渴��
void my_memclear(char *head, unsigned int length);									// ���ڴ��Ϊ��λ�������ڴ�����
void *my_calloc(enum sram memx, unsigned int nitems, unsigned int size);			// ����һ����������ڴ�(�ⲿ����)
#endif
