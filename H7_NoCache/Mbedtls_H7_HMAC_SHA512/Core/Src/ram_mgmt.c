/*
**********************************************************************************************************************
*                                            	       Notes
* Filename: ram_mgmt.c
* Notes:
* Date: 2019
* Author: 崔洪玮
**********************************************************************************************************************
*/
#include "ram_mgmt.h"	  
#include "cmsis_compiler.h"

/*
**********************************************************************************************************************
*
*	内存池(64字节对齐，以适应不同场合的需求)
*
*	STM32H7 内部共有 5 个连续的内存块
*	外加上扩充的 SDRAM 一共有 6 个内存池
*	
*	AXI RAM 的内存池首地址由编译器决定，其他内存池首地址的对应于硬件的首地址
*
*	对于字节对齐：在ARM编译器中long型数占64位也就是8个字节，所以64字节对齐可以适用各种场合
* 
**********************************************************************************************************************
*/
__ALIGNED(64) unsigned char mem1base[MEM1_MAX_SIZE];	// 内部AXI RAM内存池
__ALIGNED(64) unsigned char mem2base[MEM2_MAX_SIZE] __attribute__((section(".bss.ARM.__at_0xC03D0900")));		// 外部SDRAM内存池,前面4M给LTDC用了
__ALIGNED(64) unsigned char mem3base[MEM3_MAX_SIZE] __attribute__((section(".bss.ARM.__at_0x30000000")));		// 内部SRAM1+SRAM2内存池
__ALIGNED(64) unsigned char mem4base[MEM4_MAX_SIZE] __attribute__((section(".bss.ARM.__at_0x38000000")));		// 内部SRAM4内存池
__ALIGNED(64) unsigned char mem5base[MEM5_MAX_SIZE] __attribute__((section(".bss.ARM.__at_0x20000000")));		// 内部DTCM内存池
__ALIGNED(64) unsigned char mem6base[MEM6_MAX_SIZE] __attribute__((section(".bss.ARM.__at_0x00000000")));		// 内部ITCM内存池
/*
**********************************************************************************************************************
*
*	内存管理表
*
*	Notes：
*
*	（1）内存管理表位于对应的内存池当中，一个内存块对应于一个字节的表项，
*	所以，如果内存块过小，内存管理表也会成为一个相当大的开销。
*	选取内存块大小为 64 字节，以控制内存管理表的大小在一个合适的范围内。
*	64 ：4 = 16 ：1 
*
*	（2）在单片机中每个连续内存单元的最后，要预留内存管理表的位置。
*	换句话说，可管理的内存数有一定程度的损失。
*
**********************************************************************************************************************
*/ 
unsigned int mem1mapbase[MEM1_ALLOC_TABLE_SIZE];															// 内部SRAM内存池MAP
unsigned int mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((section(".bss.ARM.__at_0xC1E10900")));		// 外部SDRAM内存池MAP
unsigned int mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((section(".bss.ARM.__at_0x3003C000")));		// 内部SRAM1+SRAM2内存池MAP
unsigned int mem4mapbase[MEM4_ALLOC_TABLE_SIZE] __attribute__((section(".bss.ARM.__at_0x3800F000")));		// 内部SRAM4内存池MAP
unsigned int mem5mapbase[MEM5_ALLOC_TABLE_SIZE] __attribute__((section(".bss.ARM.__at_0x2001E000")));		// 内部DTCM内存池MAP
unsigned int mem6mapbase[MEM6_ALLOC_TABLE_SIZE] __attribute__((section(".bss.ARM.__at_0x0000F000")));		// 内部ITCM内存池MAP 
/*
**********************************************************************************************************************
*
*	内存管理参数
*
**********************************************************************************************************************
*/	   
const unsigned int memtblsize[SRAMBANK] = {MEM1_ALLOC_TABLE_SIZE,		// 内存表大小
										   MEM2_ALLOC_TABLE_SIZE,
										   MEM3_ALLOC_TABLE_SIZE,
										   MEM4_ALLOC_TABLE_SIZE,
										   MEM5_ALLOC_TABLE_SIZE,
										   MEM6_ALLOC_TABLE_SIZE};
									
const unsigned int memblksize[SRAMBANK] = {MEM1_BLOCK_SIZE,		   		// 内存分块大小
										   MEM2_BLOCK_SIZE,
										   MEM3_BLOCK_SIZE,
										   MEM4_BLOCK_SIZE,
										   MEM5_BLOCK_SIZE,
										   MEM6_BLOCK_SIZE};					

const unsigned int memsize[SRAMBANK] = {MEM1_MAX_SIZE,			   		// 内存总大小
									    MEM2_MAX_SIZE,
									    MEM3_MAX_SIZE,
									    MEM4_MAX_SIZE,
									    MEM5_MAX_SIZE,
									    MEM6_MAX_SIZE};								
/*
**********************************************************************************************************************
*
*	内存管理控制器
*
*	Notes：
*	(*init)(unsigned char) 		   		-- 指向内存初始化 函数 的指针
*	(*perused)(unsigned char) 		   	-- 指向获取内存使用率 函数 的指针
*	unsigned char *membase[SRAMBANK] 	-- 所有 内存池 的 起始地址 构成的数组
*	unsigned int *memmap[SRAMBANK]  	-- 所有 内存管理表 的 首地址 构成的数组
*
**********************************************************************************************************************
*/
struct _m_mallco_dev mallco_dev =
{
	my_mem_init,															// 内存初始化
	my_mem_perused,															// 内存使用率
	mem1base,mem2base,mem3base,mem4base,mem5base,mem6base,					// 内存池
	mem1mapbase,mem2mapbase,mem3mapbase,mem4mapbase,mem5mapbase,mem6mapbase,// 内存管理状态表
	0,0,0,0,0,0,								  		 					// 内存管理未就绪
};							
/*
**********************************************************************************************************************
*
*	复制内存
*
*	des:	目的地址
*	src:	源地址
*	n:		需要复制的内存长度(字节为单位)
*
**********************************************************************************************************************
*/
void my_memcpy(void *des, void *src, unsigned int n) {
	
    unsigned char *xdes = des;		// 重新定义指针
	unsigned char *xsrc = src; 

    while (n--) {
		*xdes++ = *xsrc++;
	}  
} 
/*
**********************************************************************************************************************
*
*	设置内存
*
*	s:	内存首地址
*	c:	要设置的值
*	count:需要设置的内存大小(字节为单位)
*
**********************************************************************************************************************
*/
void my_memset(void *s, unsigned char c, unsigned int count) {  
	
    unsigned char *xs = s; 
	
    while (count--) {
		*xs++ = c; 
	} 
}
/*
**********************************************************************************************************************
*	
*	内存管理初始化 
* 
*	memx:	所属内存池
*
*	Notes:
*	将对应内存块的 内存管理表 所有表项 清零。
*	通过内存管理控制器，直接找到了对应内存池的内存管理表。
*
**********************************************************************************************************************
*/
void my_mem_init(enum sram memx) {  
	
    my_memset(mallco_dev.memmap[memx], 0, memtblsize[memx]*4);	// 内存状态表数据清零  
 	mallco_dev.memrdy[memx] = 1;								// 内存管理初始化OK  
}
/*
**********************************************************************************************************************
*  
*	获取内存使用率
*
*	memx:	求取内存使用率的 内存池编号
*	返回值:	使用率(扩大了10倍,0~1000,代表0.0%~100.0%)
*
*	Notes:
*	求解方法是 遍历内存池的 内存管理表的每一个表项 并统计。
*	占用率 * 10 = 被占用的内存数目 / 内存管理表的大小
*
**********************************************************************************************************************
*/
unsigned short my_mem_perused(enum sram memx) {  
	
    unsigned int used = 0;  
    unsigned int i;  
	
    for (i = 0; i < memtblsize[memx]; i++) {  
        if (mallco_dev.memmap[memx][i]) {
			used++;
		} 
    } 
	
    return (used*1000) / (memtblsize[memx]);  
} 
/*
**********************************************************************************************************************
* 
*	内存分配(内部调用)
*
*	memx:	所属内存池
*	size:	要分配的内存大小(字节)
*	返回值:	0XFFFFFFFF,	代表错误;
*			其他,		内存偏移地址;
*
*	Notes ：
*	（1）如果所需要的字节数不能被一个内存块的大小所整除，最后一个被分配的内存块不会被完全占满。
*	（2）每次寻找合适大小的连续内存区域，都从内存表的末尾开始，直到找到符合要求的一块区域
*		再一路标记回去，在内存表的对应表项中填充本次分配的内存块数目。
*
**********************************************************************************************************************
*/
unsigned int my_mem_malloc(enum sram memx, unsigned int size) { 
	
    signed long offset = 0;  
    unsigned int nmemb;					// 需要的内存块数  
	unsigned int cmemb = 0;				// 连续空内存块数
    unsigned int i; 
	 
    if(!mallco_dev.memrdy[memx]) {
		mallco_dev.init(memx);			// 未初始化,先执行初始化	
	}
	 
    if(size == 0) {	
		return 0XFFFFFFFF;				// 不需要分配	
	}

    nmemb = size / memblksize[memx];	// 获取需要分配的连续内存块数
	
	/* 如果最后不能完整的占用一个内存块 */
    if (size%memblksize[memx]) {									
		nmemb++;
	}
	
    /* 搜索整个内存控制区 */
    for(offset = memtblsize[memx] - 1; offset >= 0; offset--) {     
		if (!mallco_dev.memmap[memx][offset]) {
			cmemb++;					// 连续空内存块数增加		
		} else {
			cmemb = 0;					// 连续内存块清零
		}

		/* 找到了连续nmemb个空内存块 */
		if (cmemb == nmemb) {
            for (i = 0; i < nmemb; i++) {  
                mallco_dev.memmap[memx][offset+i] = nmemb;  // 标注内存块非空 
            }  
            return (offset * memblksize[memx]);	// 返回偏移地址  
		}
    }  
	
    return 0XFFFFFFFF;					// 未找到符合分配条件的内存块  
} 

/*
**********************************************************************************************************************
*  
*	释放内存(内部调用) 
*
*	memx:	所属内存块
*	offset:	内存地址偏移
*	返回值:
*		0,释放成功;
*		1,未进行初始化;
*		2,偏移量超出内存池范围
*
*	Notes：
*	程序原理是 通过偏移量计算内存管理表的索引，读取索引表项所存储的值，清空从此之后的表项。
*
**********************************************************************************************************************
*/  
unsigned char my_mem_free(enum sram memx,unsigned int offset)  
{  
    int i; 
    
	/* 未初始化,先执行初始化 */
    if(!mallco_dev.memrdy[memx]) {
		mallco_dev.init(memx);    
        return 1;										// 未初始化  
    }  
	
	/* 如果偏移在内存池内 */
    if(offset < memsize[memx]) {  
        int index = offset / memblksize[memx];			// 偏移所在内存块号码  
        int nmemb = mallco_dev.memmap[memx][index];		// 内存块数量
        for (i = 0; i < nmemb; i++) {  
            mallco_dev.memmap[memx][index+i] = 0; 		// 内存块对应表项清零 
        }  
        return 0;  
    } else {
		return 2; 										// 偏移超超出内存区
	} 
} 

/*
**********************************************************************************************************************
*
*	释放内存(外部调用) 
*
*	memx:	所属内存池编号
*	ptr:	内存首地址 
*
**********************************************************************************************************************
*/
void my_free(enum sram memx, void *ptr) { 
	
	unsigned int offset;
	   
	if(ptr == NULL) {
		return;					// 地址为0.
	}
	  
 	offset = (unsigned int)ptr-(unsigned int)mallco_dev.membase[memx];     
    my_mem_free(memx, offset);	// 释放内存      
}
/*
**********************************************************************************************************************
*  
*	分配内存(外部调用)
*
*	memx:	所属内存块
*	size:	内存大小(字节)
*	返回值:	分配到的内存首地址.
*
**********************************************************************************************************************
*/
void *my_malloc(enum sram memx,unsigned int size) {  
	
    unsigned int offset;
	   
	offset = my_mem_malloc(memx, size); 
	 	   	 	   
    if (offset == 0XFFFFFFFF) {
		return NULL;  
	}
    else {
		return (void*)((unsigned int)mallco_dev.membase[memx] + offset);
	}  
}
/*
**********************************************************************************************************************
*    
*	重新分配内存(外部调用)
*
*	memx:	所属内存块
*	*ptr:	旧内存首地址
*	size:	要分配的内存大小(字节)
*	返回值:	新分配到的内存首地址.
*	实现原理: 直接分配新的足够大的内存空间，复制原地址中的内容到新的地址，释放原地址中的内存空间
*
**********************************************************************************************************************
*/
void *my_realloc(enum sram memx, void *ptr, unsigned int size) {
	
    unsigned int offset; 
	
    offset = my_mem_malloc(memx, size);   
	
    if (offset == 0XFFFFFFFF) {
		return NULL;
	}     
    else {  
		if (ptr != NULL) {		
			my_memcpy((void*)((unsigned int)mallco_dev.membase[memx] + offset), ptr, size);	// 拷贝旧内存内容到新内存   
			my_free(memx, ptr);  											  				// 释放旧内存
		}
        return (void*)((unsigned int)mallco_dev.membase[memx] + offset);  					// 返回新内存首地址
    }  
}

/*
**********************************************************************************************************************
*    
*	将一段内存块清零(外部调用)
*
*	char *head:	分配内存的起始地址
*	unsigned int length: 目标内存空间大小
*	返回值:	无
*	实现原理: 由于my_free函数不会实际将内存块清零，如果第二次重复分配到该内存块
*			  其中存留的原有内容可能会对函数造成干扰，例如strlen，所以需要将对应
*			  的内存块全部清空。
*
**********************************************************************************************************************
*/
void my_memclear(char *head, unsigned int length) {
	
	if (head != NULL) {
		my_memset(head, 0, length);
	}
}


/*
**********************************************************************************************************************
*    
*	分配一段已清零的内存(外部调用)
*
*	memx:	所属内存块
*	size:	内存大小(字节)
*	返回值:	分配到的内存首地址
*
**********************************************************************************************************************
*/
void *my_calloc(enum sram memx, unsigned int nitems, unsigned int size) {
	
	void *mem = my_malloc(memx, size * nitems);
	my_memclear(mem, size);
	
	return mem;
}
