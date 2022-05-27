#ifndef __TASK_ARR_H__
#define __TASK_ARR_H__
#include "task.h"

myTCB arrqueue;

void iniarrqueue(void);
void eqbyarrtime(myTCB* task);
void raisetsk(myTCB* task,int arrtime);

#endif