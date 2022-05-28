#ifndef __SJF_H__
#define __SJF_H__

#include "schedulehook.h"

void eq_sjf(myTCB* task);
myTCB* dq_sjf(void);
void schedulebysjf(void);
void sjf_initscheduler(void);
void sjf_createTsk(myTCB* created);

extern struct scheduler sche_SJF;

#endif