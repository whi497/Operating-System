#ifndef __USERTASK_H__
#define __USERTASK_H__

#include "../myOS/userInterface.h"
#define TimeSlice 2

void MyTask1(void);
void MyTask2(void);
void MyTask3(void);
void MyTask4(void);
void ShellTask(void);

void test_prio_sche(void);//test for prio_sche
void test_rr_sche(void);

#endif