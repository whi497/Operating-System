#ifndef __SCHEDULEHOOK_H__
#define __SCHEDULEHOOK_H__

#include "task.h"

#define SCHEDULER_FCFS 0
#define SCHEDULER_SJF 1
#define SCHEDULER_PRIORITY0 2
#define SCHEDULER_RR 3

struct scheduler {
    unsigned int type;
    myTCB* (*nextTsk_func)(void);
    void (*enqueueTsk_func)(myTCB *tsk);
    myTCB* (*dequeueTsk_func)(void);
    void (*schedulerInit_func)(void);
    void (*schedule)(void);
    void (*createTsk_hook)(myTCB* created); //if set, will be call in createTsk (before tskStart)
    void (*tick_hook)(void); //if set, tick_hook will be called every tick
}sche;

myTCB rdqueuehead; //就绪队列，schedule()执行时总是取位于队头的任务执行

extern struct scheduler sche_PRIO;

//使用define避免单开一个.c文件
#define setSysScheduler(what){ \ 
    switch(what){ \
        case SCHEDULER_FCFS: break; \
        case SCHEDULER_SJF: break; \
        case SCHEDULER_PRIORITY0: { \
            sche = sche_PRIO; \
            sche.schedulerInit_func(); \
        }break; \
        case SCHEDULER_RR: break; \
        default: break; \
    } \
}


#endif
