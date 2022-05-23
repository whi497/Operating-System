#include "../../include/myPrintk.h"
// 一个EEB表示一个空闲可用的Block
typedef struct EEB {
	unsigned long next_start;
	unsigned long state;
}EEB;	//占8个字节

#define EEB_size 8

void showEEB(struct EEB *eeb){
	if(eeb->state == 1){
		myPrintk(0x7,"EEB(start=0x%x, next=0x%x ,state=Alloced)\n", eeb, eeb->next_start);
	}
	else if(eeb->state == 0){
		myPrintk(0x7,"EEB(start=0x%x, next=0x%x ,state=Free)\n", eeb, eeb->next_start);
	}
}

//eFPartition是表示整个内存的数据结构
typedef struct eFPartition{
	unsigned long totalN;
	unsigned long perSize;  //unit: byte	
	unsigned long firstFree;
}eFPartition;	//占12个字节

#define eFPartition_size 12

void showeFPartition(struct eFPartition *efp){
	myPrintk(0x5,"eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x, firstFree=0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstFree);
}

void eFPartitionWalkByAddr(unsigned long efpHandler){
	//本函数需要实现！！！
	//todo//done
	/*功能：本函数是为了方便查看和调试的。
	1、打印eFPartiiton结构体的信息，可以调用上面的showeFPartition函数。
	2、遍历每一个EEB，打印出他们的地址以及下一个EEB的地址（可以调用上面的函数showEEB）

	*/
	eFPartition* mem_pointer = (eFPartition*)efpHandler;
	showeFPartition(mem_pointer);
	unsigned long check_addr = efpHandler + eFPartition_size;
	EEB* eebPointer = (EEB*)check_addr;
	for(int i = 0; i<mem_pointer->totalN ; i++) {
		showEEB(eebPointer);
		check_addr = check_addr + EEB_size + mem_pointer->perSize;
		eebPointer = (EEB*)check_addr;
	}

}


unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n){
	//本函数需要实现！！！
	//todo//done
	/*功能：计算占用空间的实际大小，并将这个结果返回
	1.根据参数persize（每个大小）和n个数计算总大小，注意persize的对齐。
		例如persize是31字节，你想8字节对齐，那么计算大小实际代入的一个块的大小就是32字节。
	2.同时还需要注意“隔离带”EEB的存在也会占用4字节的空间。
		typedef struct EEB {
			unsigned long next_start;
		}EEB;	
	3.最后别忘记加上eFPartition这个数据结构的大小，因为它也占一定的空间。

	*/
	int finalpersize = (perSize/8)*8+((perSize%8)?8:0);//8 bytes align
	return (finalpersize*n+EEB_size*n+eFPartition_size);

}

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n){
	//本函数需要实现！！！
	//todo//done
	/*功能：初始化内存
	1.需要创建一个eFPartition结构体，需要注意的是结构体的perSize不是直接传入的参数perSize，需要对齐。结构体的next_start也需要考虑一下其本身的大小。
	2.就是先把首地址start开始的一部分空间作为存储eFPartition类型的空间
	3.然后再对除去eFPartition存储空间后的剩余空间开辟若干连续的空闲内存块，将他们连起来构成一个链。注意最后一块的EEB的nextstart应该是0
	4.需要返回一个句柄，也即返回eFPartition *类型的数据
	注意的地方：
		1.EEB类型的数据的存在本身就占用了一定的空间。
	*/
	int finalpersize = (perSize/8)*8+((perSize%8)?8:0);
	eFPartition* Ini_eFPartition= (eFPartition*)start;
	Ini_eFPartition->totalN = n;
	Ini_eFPartition->perSize = finalpersize;
	Ini_eFPartition->firstFree = start + 12;
	unsigned long check_addr = start + 12;
	EEB* epointer;
	for (int i=0; i< n; i++){//循环初始化隔离带
		EEB* epointer = (EEB*)(check_addr);
		epointer->next_start = check_addr + EEB_size + finalpersize;
		epointer->state = 0;
		if(i == n-1) epointer->next_start = 0;
		// myPrintk(0x7,"%x %x %x\n",check_addr,epointer->next_start,finalpersize);//debug
		// epointer = (EEB*)epointer->next_start;
		check_addr += finalpersize + EEB_size;
	}
	return (unsigned long)Ini_eFPartition;
}


unsigned long eFPartitionAlloc(unsigned long EFPHandler){
	//本函数需要实现！！！
	//todo//done
	/*功能：分配一个空间
	1.本函数分配一个空闲块的内存并返回相应的地址，EFPHandler表示整个内存的首地址
	2.事实上EFPHandler就是我们的句柄，EFPHandler作为eFPartition *类型的数据，其存放了我们需要的firstFree数据信息
	3.从空闲内存块组成的链表中拿出一块供我们来分配空间，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EEB类型的数据的存在本身就占用了一定的空间。

	*/
	eFPartition* block_pointer = (eFPartition*) EFPHandler;
	if(!block_pointer->firstFree) {
		myPrintk(12,"Error:No free block available\n");
		return 0;
	}

	EEB* distri_block = (EEB*) block_pointer->firstFree;
	distri_block->state = 1;
	block_pointer->firstFree = distri_block->next_start;

	return (unsigned long)distri_block+EEB_size;//除去EEB
}


unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart){
	//本函数需要实现！！！
	//todo//done
	/*功能：释放一个空间
	1.mbstart将成为第一个空闲块，EFPHandler的firstFree属性也需要相应大的更新。
	2.同时我们也需要更新维护空闲内存块组成的链表。
	
	*/
	eFPartition* block_pointer = (eFPartition*) EFPHandler;
	EEB* blockfree = (EEB*) (mbStart-EEB_size) ;
	
	blockfree->next_start = block_pointer->firstFree;//add the free block to the free list
	blockfree->state = 0;
	block_pointer->firstFree = mbStart - EEB_size;

	return EFPHandler;
}
