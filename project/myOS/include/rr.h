#ifndef __RR_H__
#define __RR_H__

#include "schedulehook.h"

void eq_rr(myTCB* task);
myTCB* dq_rr(void);
void schedulebyRR(void);
void rr_tickhook(void);
void rr_initscheduler(void);
void rr_createTsk(myTCB* created);
void rr_tskendhook(void);
extern struct scheduler sche_RR;

#endif