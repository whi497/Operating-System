#ifndef __PRIORITY_H__
#define __PRIORITY_H__

#include "schedulehook.h"

void eqbypriority(myTCB* task);
myTCB* dqbypriority(void);
void schedulebypriority(void);
void pri_tickhook(void);
void pri_initscheduler(void);
void pri_createTsk(myTCB* created);

extern struct scheduler sche_PRIO;

#endif

