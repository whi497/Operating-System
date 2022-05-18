#include "../../include/myPrintk.h"


//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition{
	unsigned long totalSize;//用于判断回收是否在内存范围内
	unsigned long size;
	unsigned long firstFreeStart; 
} dPartition;	//共占12个字节

#define dPartition_size ((unsigned long)0xC)

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB{
	unsigned long size;
	unsigned long state;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, blongs to user
	};	                           
} EMB;	//共占12个字节

#define EMB_size ((unsigned long)0xc)

void showEMB(struct EMB * emb){
	// myPrintk(0x7,"%x %x\n",&emb->nextStart,sizeof(EMB));	
	if(emb->state == 1)
		myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x, state=Alloced)\n", emb, emb->size, emb->nextStart);
	else if(emb->state == 0)
		myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x, state=Free)\n", emb, emb->size, emb->nextStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	//本函数需要实现！！！
	//todo//done
	/*功能：初始化内存。
	1.在地址start处，首先是要有dPartition结构体表示整个数据结构(也即句柄)。
	2.然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	3.返回start首地址(也即句柄)。
	注意有两个地方的大小问题：
		第一个是由于内存肯定要有一个EMB和一个dPartition，totalSize肯定要比这两个加起来大。
		第二个注意EMB的size属性不是totalsize，因为dPartition和EMB自身都需要要占空间。
	
	*/
	if(totalSize < dPartition_size+EMB_size){
		myPrintk(12,"Error:dynamic block too small\n");
		return 0;
	}
	dPartition* dynamic_block = (dPartition*) start;
	dynamic_block->totalSize = totalSize;
	dynamic_block->size = totalSize;	//initial the dPartition
	dynamic_block->firstFreeStart = start + dPartition_size;
	EMB* dy_block = (EMB*)(start+dPartition_size);
	dy_block->nextStart = 0;
	dy_block->state = 0;
	dy_block->size = (totalSize - dPartition_size - EMB_size);
	return (unsigned long)dynamic_block;
}

void dPartitionWalkByAddr(unsigned long dp){
	//本函数需要实现！！！
	//todo//done
	/*功能：本函数遍历输出EMB 方便调试
	1.先打印dP的信息，可调用上面的showdPartition。
	2.然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息

	*/
	dPartition* mem_pointer = (dPartition*)dp;
	showdPartition(mem_pointer);
	unsigned long check_addr = dp + dPartition_size;
	EMB* embPointer = (EMB*)check_addr;
	do {//loop through all emb
		showEMB(embPointer);
		check_addr = check_addr + EMB_size + embPointer->size;//按地址大小找到下一个EMB
		embPointer = (EMB*)check_addr;
	}while(check_addr<dp+mem_pointer->totalSize);

}

void showpartofEMB(unsigned long dp){//用于测试kmalloc和malloc的分离，输出跳过用于cmd的小块emb
	dPartition* mem_pointer = (dPartition*)dp;
	showdPartition(mem_pointer);
	unsigned long check_addr = dp + dPartition_size;
	EMB* embPointer = (EMB*)check_addr;
	int n=0;
	do {
		if(n<3 | (embPointer->size>=0x100))
			showEMB(embPointer);
		if(n==3) myPrintk(0x3,"...\n");
		check_addr = check_addr + EMB_size + embPointer->size;//按地址大小找到下一个EMB
		embPointer = (EMB*)check_addr;
		n++;
	}while(check_addr<dp+mem_pointer->totalSize);
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	//本函数需要实现！！！
	//todo//done
	/*功能：分配一个空间
	1.使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
	2.成功分配返回首地址，不成功返回0
	3.从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于size，我们还将把剩余部分放回链表中)，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EMB类型的数据的存在本身就占用了一定的空间。

	*/
	int distrui_flag = 0;
	int finalsize = (size/8)*8+((size%8)?8:0);//8 bytes align
	// myPrintk(0x7,"finalsize=%d\n",finalsize);//debug
	dPartition* dynamic_block = (dPartition*) dp;
	if(!dynamic_block->firstFreeStart){
		myPrintk(12,"Error: no free dynamic mem avalible\n");\
		return 0;
	}
	
	unsigned long findaddr = dynamic_block->firstFreeStart;
	EMB* dy_block = (EMB*)findaddr;
	do{
		if(dy_block->size>=finalsize){//space enough
			if(dy_block->size-finalsize<EMB_size){//分配完多余剩余空间不足一个emb，直接全部分配
			    myPrintk(0x7,"here");
				dynamic_block->firstFreeStart = dy_block->nextStart;
				dynamic_block->size = dynamic_block->size - dy_block->size;
				dy_block->state = 1;
				distrui_flag = 1;
				break;
			}
			else{//否则分配相应大小，并另起一个emb
				EMB* Insertemb = (EMB*)(dynamic_block->firstFreeStart+EMB_size+finalsize);
				Insertemb->size = dy_block->size-finalsize-EMB_size;
				Insertemb->nextStart = dy_block->nextStart;
				dy_block->size = finalsize;
				dy_block->state = 1;
				dy_block->nextStart = (unsigned long)Insertemb;
				dynamic_block->firstFreeStart = (unsigned long)Insertemb;
				dynamic_block->size = dynamic_block->size - finalsize - EMB_size;
				distrui_flag = 1;
				break;
			}
		}
	}while(dy_block->nextStart);

	if(distrui_flag) return ((unsigned long)dy_block + 8);//保留EMB的size和state信息
	
	// myPrintk(14,"no suit block found\n");
	return 0;
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	//todo//done
	/*功能：释放一个空间
	1.按照对应的fit的算法释放空间
	2.注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回1 没问题
		返回0 error
	3.需要考虑两个空闲且相邻的内存块的合并
	*/
	start -= 8;//找到该块对应的emb
	dPartition* dynamic_block = (dPartition*)dp;
	EMB* blockfree = (EMB*)(start);
	
	if(start<(dp+dPartition_size) | ((start+EMB_size+blockfree->size)>(dp+dynamic_block->totalSize))){
		myPrintk(12,"Error:illegal block to be free\n");
		return 0;
	}
	// myPrintk(0x7,"%x %x %x %x",start,dp+dPartition_size,start+EMB_size+blockfree->size,dp+dynamic_block->size);
	blockfree->state = 0;
	
	unsigned long indexaddr = dynamic_block->firstFreeStart;
	unsigned long preaddr = 0;
	unsigned long nextaddr = 0;
	EMB* index_block = (EMB*)indexaddr;
	do{
		if(indexaddr<start) preaddr = indexaddr;//反复更新待释放块地址前的空闲块
		if(indexaddr>start) {//确定待释放块地址后最近一个空闲块和前最近一个空闲块
			nextaddr = indexaddr;
			if(nextaddr != 0){
				EMB* next_block = (EMB*)nextaddr;
				if(start+blockfree->size+EMB_size == nextaddr){//merge block free
					dynamic_block->size += EMB_size + blockfree->size;
					blockfree->nextStart = next_block->nextStart;
					blockfree->size += next_block->size + EMB_size;
				}
				else{
					dynamic_block->size += blockfree->size;
					blockfree->nextStart = (unsigned long)next_block;
				}
			}
			else {//释放块在尾部
				dynamic_block->size += blockfree->size;
				blockfree->nextStart = 0;
			}
			if(preaddr != 0){
				EMB* pre_block = (EMB*)preaddr;
				if(preaddr+pre_block->size+EMB_size == start) {//merge block free
					pre_block->nextStart = blockfree->nextStart;
					pre_block->size += blockfree->size + EMB_size;
					dynamic_block->size += EMB_size;
				}
				else
					pre_block->nextStart = (unsigned long)blockfree;
			}
			else //释放块在最前面
				dynamic_block->firstFreeStart = (unsigned long)blockfree;
			break;
		}
		indexaddr = index_block->nextStart;
		index_block = (EMB*)indexaddr;
	}while(indexaddr);
	// myPrintk(0x7,"%x %x\n",preaddr,nextaddr);//debug 
	// myPrintk(0x7,"%x\n",blockfree->size);
	return 1;
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
