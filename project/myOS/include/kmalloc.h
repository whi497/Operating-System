#ifndef __KMALLOC_H__
#define __KMALLOC_H__

#include "mem.h"
//todo//done

//需要实现！！！（可选）
	/*
	这里实现kmalloc/kfree，调用dp或者是efp里的函数实现即可
	*/
//实现于kmalloc.c避免引用头文件导致重定义（与malloc.c在同一个文件夹下）
unsigned long kmalloc(unsigned long size);

unsigned long kfree(unsigned long start);
#endif