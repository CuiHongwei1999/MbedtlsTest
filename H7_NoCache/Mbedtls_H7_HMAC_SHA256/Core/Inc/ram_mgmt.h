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
	SRAMIN, 	//  AXI�ڴ��,AXI��512KB 
	SRAMEX, 	//  �ⲿ�ڴ��(SDRAM),SDRAM��32MB
	SRAM12, 	//  SRAM1/2/3�ڴ��,SRAM1+SRAM2,��256KB
	SRAM4, 		//  SRAM4�ڴ��,SRAM4��64KB
	SRAMDTCM, 	//  DTCM�ڴ��,DTCM��128KB,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!!
	SRAMITCM, 	//  ITCM�ڴ��,DTCM��64 KB,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!!
};

#define SRAMBANK 		6	// ����֧�ֵ�SRAM����.	
#define MEM_BLOCK_SIZE	64	// ͳһ�����ڴ���С

// mem1�ڴ�����趨.mem1��H7�ڲ���AXI�ڴ�.
#define MEM1_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			448*1024  						// �������ڴ� 448K,H7��AXI�ڴ��ܹ�512KB
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	// �ڴ���С

// mem2�ڴ�����趨.mem2���ⲿ��SDRAM�ڴ�
#define MEM2_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			26880 *1024  					// �������ڴ�26880K,����SDRAM�ܹ�32MB,LTDCռ��4MB,��ʣ28MB.
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	// �ڴ���С
		 
// mem3�ڴ�����趨.mem3��H7�ڲ���SRAM1+SRAM2�ڴ�
#define MEM3_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			240 *1024  						// �������ڴ�240K,H7��SRAM1+SRAM2��256KB
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	// �ڴ���С
		 
// mem4�ڴ�����趨.mem4��H7�ڲ���SRAM4�ڴ�
#define MEM4_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM4_MAX_SIZE			60 *1024  						// �������ڴ�60K,H7��SRAM4��64KB
#define MEM4_ALLOC_TABLE_SIZE	MEM4_MAX_SIZE/MEM4_BLOCK_SIZE 	// �ڴ���С
		 
// mem5�ڴ�����趨.mem5��H7�ڲ���DTCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!!
#define MEM5_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM5_MAX_SIZE			120 *1024  						// �������ڴ�120K,H7��DTCM��128KB
#define MEM5_ALLOC_TABLE_SIZE	MEM5_MAX_SIZE/MEM5_BLOCK_SIZE 	// �ڴ���С
		 
// mem6�ڴ�����趨.mem6��H7�ڲ���ITCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!!
#define MEM6_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// �ڴ���СΪ64�ֽ�
#define MEM6_MAX_SIZE			60 *1024  						// �������ڴ�60K,H7��ITCM��64KB
#define MEM6_ALLOC_TABLE_SIZE	MEM6_MAX_SIZE/MEM6_BLOCK_SIZE 	// �ڴ���С
		 


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
