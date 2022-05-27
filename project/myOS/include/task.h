#ifndef __TASK_H__
#define __TASK_H__
#define NULL (void*)0
#define UNKNOWN -1

#define initTskBdy myMain//对接main

#define KSTKSIZE 0x2000
#define TSK_STK_SIZE 0x1000
#define TotalTCB 10
#define MAXTSKSUM 10

//different option for para
#define PRIORITY 1
#define EXETIME 2
#define ARRTIME 3
#define SCHED_POLICY 4

//state of a task
#define FREE -1
#define IDLE 0
#define READY 1
#define RUNNING 2

typedef struct tskPara {
    unsigned int priority;
    unsigned int arrTime;
    unsigned int exeTime;
    unsigned int schedPolicy;
}tskPara;
void initTskPara(tskPara **buffer);
void setTskPara(unsigned int option, unsigned int value, tskPara *buffer);
unsigned int getTskPara(unsigned int option, tskPara *para);

typedef struct TCB{
    unsigned int id;
    unsigned int state;
    unsigned long* tskptr;//栈指针
    unsigned long* stktop;//栈顶
    unsigned long stksize;
    void (* task_func)(void);
    tskPara* para;
    struct TCB* nexttcb;
}myTCB;

myTCB** tcb_pool;
//manager all tcb

myTCB* freetcb;
myTCB* inittcb;
myTCB* idletcb;
myTCB* runningtcb;
//keep the important task's tid

void initTskBdy(void);
void idleTskBdy(void);

//---------------------------general management func---------------------------//
unsigned long **prevTSK_StackPtrAddr;
unsigned long *nextTSK_StackPtr;

void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);
void stack_init(unsigned long **stk, void (*task)(void));

unsigned long BspContextBase[KSTKSIZE];
unsigned long *BspContext;
//构建初始上下文环境

int createTsk(void (*tskBody)(void));
void destroyTsk(int tskIndex);

void tskStart(myTCB *tsk);
void tskEnd(void);

void iniTskManager(void);

#endif