#ifndef __KMALLOC_H__
#define __KMALLOC_H__

#include "mem.h"
//todo//done

//需要实现！！！（可选）
	/*
	这里实现kmalloc/kfree，调用dp或者是efp里的函数实现即可
	*/
unsigned long kmalloc(unsigned long size) {//used for kernal
    return dPartitionAlloc(kpMemHandler, size);
}

unsigned long kfree(unsigned long start) {//used for kernal
    return dPartitionFree(kpMemHandler, start);
}
#endif