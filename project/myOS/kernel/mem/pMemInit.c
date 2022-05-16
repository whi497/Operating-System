#include "../../include/myPrintk.h"
#include "../../include/mem.h"
unsigned long MemStart;
unsigned long MemSize;
unsigned long kMemStart;
unsigned long kMemSize;
unsigned long pMemStart;//可用的内存的起始地址
unsigned long pMemSize;//可用的大小

void memTest(unsigned long start, unsigned long grainSize){
	/*本函数需要实现！！！*/
	//todo
	/*功能：检测算法
		这一个函数对应实验讲解ppt中的第一大功能-内存检测。
		本函数的功能是检测从某一个地址开始的内存是否可用，具体算法就可以用ppt上说的写了读看是否一致。
	注意点两个：
	1、开始的地址要大于1M，需要做一个if判断。
	2、grainsize不能太小，也要做一个if判断
	*/

	// if (start < 0x100000)
	// 	start = 0x100000;
	// if (grainSize < 2)
	// 	grainSize = 2;

	// unsigned long addr = start;
    // unsigned short data;
    // unsigned short *addr_head, *addr_tail;
    // unsigned short test_data1 = 0xAA55;
    // unsigned short test_data2 = 0x55AA;
	// int flag_fail = 0; // whether the test fails
	// MemSize = 0;
	// MemStart = start;

	// // get the size of pMem
    // while (!flag_fail) {
    //     flag_fail = 0;

    //     addr_head = (unsigned short *)addr; // the first 2 bytes of current grain
    //     addr_tail = (unsigned short *)(addr + grainSize - 2); // the last 2 bytes of current grain

    //     data = *addr_head; // read original data
    //     *addr_head = test_data1; // write 0xAA55
    //     if (*addr_head != test_data1)
    //         flag_fail = 1;
    //     *addr_head = test_data2; // write 0x55AA
    //     if (*addr_head != test_data2)
    //         flag_fail = 1;
    //     *addr_head = data; // write back original data

    //     data = *addr_tail; // read original data
    //     *addr_tail = test_data1; // write 0xAA55
    //     if (*addr_tail != test_data1)
    //         flag_fail = 1;
    //     *addr_tail = test_data2; // write 0x55AA
    //     if (*addr_tail != test_data2)
    //         flag_fail = 1;
    //     *addr_tail = data; // write back original data
        
    //     if (!flag_fail) {
    //         addr += grainSize;
    //         MemSize += grainSize;
    //     }
    // }
	
	// myPrintk(0x7, "MemStart: %x\n", MemStart);
	// myPrintk(0x7, "MemSize:  %x\n", MemSize);

	
	if(start<0x100000) {
		myPrintk(12,"Error:illegal start address\n");
		start = 0x100000;
	}
	else if(grainSize<2) {
		myPrintk(14,"Warning:grainSize too small\n");
		grainSize = 2;
	}
	MemStart = start;
	unsigned long grain_head = start;
	unsigned long grain_tail = start+grainSize;

	unsigned short * pwrite;
	unsigned short hold;
	while(1) {//todo:to be modified
		pwrite = (unsigned short *) grain_head;
		//test grainhead
		hold = *pwrite;
		*pwrite = 0xAA55;
		if(*pwrite != 0xAA55) break;
		*pwrite = 0x55AA;
		if(*pwrite != 0x55AA) break;
		*pwrite = hold;
		pwrite = (short int*) (grain_tail-2);
		//test graintail
		hold = *pwrite;
		*pwrite = 0xAA55;
		if(*pwrite != 0xAA55) break;
		*pwrite = 0x55AA;
		if(*pwrite != 0x55AA) break;
		*pwrite = hold;

		grain_head += grainSize;
		grain_tail += grainSize;
	}
	MemSize = grain_tail - start;
	myPrintk(0x7,"MemStart: %x  \n", MemStart);
	myPrintk(0x7,"MemSize:  %x  \n", MemSize);
}

extern unsigned long _end;
void MemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(_end_addr,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
}

void kMemInit(void){//initial the kernal memory
	kMemStart = MemStart;
	kMemSize = MemSize>>2;
	myPrintk(0x7,"Kernal memory start at: %x, size: %x\n",kMemStart,kMemSize);
	kpMemHandler = dPartitionInit(kMemStart,kMemSize);
}

void pMemInit(void){
	pMemStart = kMemStart + kMemSize + 0x1000;//加0x1000缓冲防止溢出
	pMemSize = MemSize - kMemSize - 0x1000;
	myPrintk(0x7,"User memory start at: %x, size: %x\n",pMemStart,pMemSize);
	pMemHandler = dPartitionInit(pMemStart,pMemSize);	
}
