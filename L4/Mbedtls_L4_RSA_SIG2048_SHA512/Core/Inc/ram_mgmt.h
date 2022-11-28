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
	SRAM1, 	//  SRAM1
	SRAM2, 	//  SRAM2
};

#define SRAMBANK 		2	// 定义支持的SRAM块数.	
#define MEM_BLOCK_SIZE	64	// 统一定义内存块大小

// mem1内存参数设定.mem1是H7内部的AXI内存.
#define MEM1_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM1_MAX_SIZE			60 * 1024  						// total memory size: 96K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	// 内存表大小

// mem2内存参数设定.mem2是外部的SDRAM内存
#define MEM2_BLOCK_SIZE			MEM_BLOCK_SIZE  	  			// 内存块大小为64字节
#define MEM2_MAX_SIZE			30 * 1024  						// total memory size
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	// 内存表大小	 


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
