/*
**********************************************************************************************************************
*                                            	       Notes
* Filename: ram_mgmt.h
* Notes:
* Date: 2019
* Author: 崔洪玮
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
// 定义六个内存池
enum sram {
	SRAMIN, 	//  AXI内存池,AXI共512KB 
	SRAMEX, 	//  外部内存池(SDRAM),SDRAM共32MB
	SRAM12, 	//  SRAM1/2/3内存池,SRAM1+SRAM2,共256KB
	SRAM4, 		//  SRAM4内存池,SRAM4共64KB
	SRAMDTCM, 	//  DTCM内存池,DTCM共128KB,此部分内存仅CPU和MDMA(通过AHBS)可以访问!!!!
	SRAMITCM, 	//  ITCM内存池,DTCM共64 KB,此部分内存仅CPU和MDMA(通过AHBS)可以访问!!!!
};

#define SRAMBANK 		6	// 定义支持的SRAM块数.	
#define MEM_BLOCK_SIZE	64	// 统一定义内存块大小

// mem1内存参数设定.mem1是H7内部的AXI内存.
#define MEM1_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM1_MAX_SIZE			448*1024  						// 最大管理内存 448K,H7的AXI内存总共512KB
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	// 内存表大小

// mem2内存参数设定.mem2是外部的SDRAM内存
#define MEM2_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM2_MAX_SIZE			26880 *1024  					// 最大管理内存26880K,外扩SDRAM总共32MB,LTDC占了4MB,还剩28MB.
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	// 内存表大小
		 
// mem3内存参数设定.mem3是H7内部的SRAM1+SRAM2内存
#define MEM3_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM3_MAX_SIZE			240 *1024  						// 最大管理内存240K,H7的SRAM1+SRAM2共256KB
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	// 内存表大小
		 
// mem4内存参数设定.mem4是H7内部的SRAM4内存
#define MEM4_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM4_MAX_SIZE			60 *1024  						// 最大管理内存60K,H7的SRAM4共64KB
#define MEM4_ALLOC_TABLE_SIZE	MEM4_MAX_SIZE/MEM4_BLOCK_SIZE 	// 内存表大小
		 
// mem5内存参数设定.mem5是H7内部的DTCM内存,此部分内存仅CPU和MDMA可以访问!!!!!!
#define MEM5_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM5_MAX_SIZE			120 *1024  						// 最大管理内存120K,H7的DTCM共128KB
#define MEM5_ALLOC_TABLE_SIZE	MEM5_MAX_SIZE/MEM5_BLOCK_SIZE 	// 内存表大小
		 
// mem6内存参数设定.mem6是H7内部的ITCM内存,此部分内存仅CPU和MDMA可以访问!!!!!!
#define MEM6_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM6_MAX_SIZE			60 *1024  						// 最大管理内存60K,H7的ITCM共64KB
#define MEM6_ALLOC_TABLE_SIZE	MEM6_MAX_SIZE/MEM6_BLOCK_SIZE 	// 内存表大小
		 


// 内存管理控制器
struct _m_mallco_dev
{
	void (*init)(enum sram);						// 初始化
	unsigned short (*perused)(enum sram);			// 内存使用率
	unsigned char 	*membase[SRAMBANK];				// 内存池 管理SRAMBANK个区域的内存
	unsigned int 	*memmap[SRAMBANK]; 				// 内存管理状态表
	unsigned char  	memrdy[SRAMBANK]; 				// 内存管理是否就绪
};

/*
*********************************************************************************************************
*                                              FUNCTIONS
*********************************************************************************************************
*/
// 用户调用函数
void my_free(enum sram memx, void *ptr);  											// 内存释放(外部调用)
void *my_malloc(enum sram memx, unsigned int size);									// 内存分配(外部调用)
void *my_realloc(enum sram memx, void *ptr, unsigned int size);						// 重新分配内存(外部调用)
unsigned short my_mem_perused(enum sram memx) ;										// 获得内存使用率(外/内部调用) 
void my_mem_init(enum sram memx);													// 内存管理初始化函数(外/内部调用)
void my_memset(void *s, unsigned char c, unsigned int count);  						// 批量设置一段内存中的值
void my_memcpy(void *des, void *src, unsigned int n);								// 内存复制
void my_memclear(char *head, unsigned int length);									// 以内存块为单位的批量内存清零
void *my_calloc(enum sram memx, unsigned int nitems, unsigned int size);			// 分配一段已清零的内存(外部调用)
#endif
