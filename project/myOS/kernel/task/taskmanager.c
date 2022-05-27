#include "../../include/task.h"
#include "../../include/task_arr.h"
#include "../../include/kmalloc.h"
#include "../../include/schedulehook.h"
#include "../../include/irq.h"
#include "../../include/myPrintk.h"
#include "../../include/priority.h"

int schepolicy = 3;
//===============================task para management============================//
void initTskPara(tskPara **buffer){
    *buffer = (tskPara*)kmalloc(sizeof(tskPara));
    (*buffer)->priority = 0;
    (*buffer)->arrTime = 0;
    (*buffer)->exeTime = 0;
    (*buffer)->schedPolicy = UNKNOWN;
}

void setTskPara(unsigned int option, unsigned int value, tskPara *buffer){
   switch(option){
        case PRIORITY: buffer->priority = value; break;
        case EXETIME: buffer->exeTime = value; break;
        case ARRTIME: buffer->arrTime = value; break;
        case SCHED_POLICY: buffer->schedPolicy = value; break;
        default: return;
    } 
}

unsigned int getTskPara(unsigned int option, tskPara *para){
    switch(option){
        case PRIORITY: return para->priority;
        case EXETIME: return para->exeTime;
        case ARRTIME: return para->arrTime;
        case SCHED_POLICY: return para->schedPolicy;
        default: return -1;
    }
    return -1;
}


//================================task management================================//
void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk) {
    prevTSK_StackPtrAddr = prevTskStkAddr;
    nextTSK_StackPtr = nextTskStk;
    CTX_SW();
}

void stack_init(unsigned long **stk, void (*task)(void)) {
    *(*stk)-- = (unsigned long)0x08; // CS
    *(*stk)-- = (unsigned long)tskEnd; 
    *(*stk)-- = (unsigned long)task; // eip
    // pushf
    *(*stk)-- = (unsigned long)0x0202; // flag registers
    // pusha
    *(*stk)-- = (unsigned long)0xAAAAAAAA; // eax
    *(*stk)-- = (unsigned long)0xCCCCCCCC; // ecx
    *(*stk)-- = (unsigned long)0xDDDDDDDD; // edx
    *(*stk)-- = (unsigned long)0xBBBBBBBB; // ebx
    *(*stk)-- = (unsigned long)0x44444444; // esp
    *(*stk)-- = (unsigned long)0x55555555; // ebp
    *(*stk)-- = (unsigned long)0x66666666; // esi
    **stk = (unsigned long)0x77777777; // edi
}

int createTsk(void (*tskBody)(void)){
    disable_interrupt();

    if(freetcb == NULL)
        return -1;
    myTCB* tsk = freetcb;
    freetcb = freetcb->nexttcb;
    tsk->state = IDLE;
    tsk->stktop = (unsigned long*)kmalloc(TSK_STK_SIZE);
    tsk->tskptr = tsk->stktop + TSK_STK_SIZE - 1;
    // myPrintk(0x7,"%d %d\n",(unsigned long)tsk->stktop,(unsigned long)tsk->taskptr);//debug
    tsk->stksize = TSK_STK_SIZE;
    tsk->runtime = 0;
    tsk->task_func = tskBody;
    initTskPara(&(tsk->para));
    tsk->nexttcb = NULL;
    // sche.createTsk_hook(tsk);//todo
    stack_init(&(tsk->tskptr), tskBody);
    enable_interrupt();

    return tsk->id;
}

void destroyTsk(int tskIndex){
    disable_interrupt();

    if(tskIndex == UNKNOWN){
        enable_interrupt();
        return;
    }
    tcb_pool[tskIndex]->nexttcb = freetcb;
    freetcb = tcb_pool[tskIndex];
    tcb_pool[tskIndex]->state = FREE;
    kfree((unsigned long)(tcb_pool[tskIndex]->stktop));
    tcb_pool[tskIndex]->tskptr = NULL;
    tcb_pool[tskIndex]->stktop = NULL;
    tcb_pool[tskIndex]->stksize = 0;
    tcb_pool[tskIndex]->task_func = NULL;
    kfree((unsigned long)(tcb_pool[tskIndex]->para));
    
    enable_interrupt();
}

void tskStart(myTCB *tsk){
    tsk->state = READY;
    sche.enqueueTsk_func(tsk);
}

void tskEnd(void){
    unsigned long* ptr = runningtcb->tskptr;
    destroyTsk(runningtcb->id);//对于抢占式
    runningtcb = NULL;
    context_switch(&ptr, BspContext);
}

void startMultitask(void) {
    BspContext = BspContextBase + KSTKSIZE - 1;
    // myPrintk(0x7,"here\n");//debug
    while(1){
        sche.schedule();
    }
}

void idleTskBdy(void){
   	myPrintk(0x7, "=================IdleTask=================\n");
	myPrintk(0x7, "           Waitting to schedule...      \n\n");
    while(1)
        sche.schedule();
    return ;
}

void iniTskManager(void){
    setSysScheduler(schepolicy);

    freetcb = NULL;
    inittcb = NULL;
    idletcb = NULL;
    runningtcb = NULL;
    myTCB* ptr = NULL;
    myTCB* preptr = NULL;
    for(int i=0; i<TotalTCB; i++){
        ptr = (myTCB*)kmalloc(sizeof(myTCB));
        ptr->id = i;
        ptr->state = FREE;
        ptr->tskptr = NULL;
        ptr->stktop = NULL;
        ptr->stksize = 0;
        ptr->task_func = NULL;
        ptr->para = NULL;
        ptr->nexttcb = NULL;
        if(preptr){
            preptr->nexttcb = ptr;
        }
        preptr = ptr;
        tcb_pool[i] = ptr;
    }
    freetcb = tcb_pool[0];

    // for(int i = 0; i < TotalTCB; i++){
    //     myPrintk(0x7,"%d\n",tcb_pool[i]->id);
    // }
    // while(1);

    int inittid = createTsk(initTskBdy);
    inittcb = tcb_pool[inittid];
    setTskPara(PRIORITY,4,inittcb->para);
    // myPrintk(0x7,"initsk id = %d\n",inittcb->id);
    int idletid = createTsk(idleTskBdy);
    idletcb = tcb_pool[idletid];
    setTskPara(PRIORITY, 0, idletcb->para);
    // myPrintk(0x7,"idletsk id = %d\n",idletcb->id);
    
    launchtsk(tcb_pool[inittcb->id],0);
    launchtsk(tcb_pool[idletcb->id],0);
    //myPrintk(0x7,"here0")
    // while(1);//debug
    startMultitask();
}


