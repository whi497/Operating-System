#include "../../include/rr.h"
#include "../../include/myPrintk.h"
#include "../../include/irq.h"
#include "../../include/taskclock.h"
#include "../../include/task_arr.h"
#include "../../../userApp/usertask.h"

int timeslice;
int counter=0;

struct scheduler sche_RR = {
    SCHEDULER_RR,
    NULL,
    eq_rr,
    dq_rr,
    rr_initscheduler,
    schedulebyRR,
    rr_createTsk,
    rr_tskendhook,
    rr_tickhook,
};

void rr_initscheduler(void) {
    rdqueuehead.nexttcb = NULL;
    timeslice = TimeSlice;
    addTaskClockHook(rr_tickhook);
}

void eq_rr(myTCB* task){
    myTCB* ptr = rdqueuehead.nexttcb;
    myTCB* preptr = &rdqueuehead;

    disable_interrupt();//入队保护
    // myPrintk(0x7, "eq priority = %d\n", task->para->priority);//debug
    if(!rdqueuehead.nexttcb){
        rdqueuehead.nexttcb = task;
        task->nexttcb = NULL;
    }
    else{
        while(ptr){
            preptr = ptr;
            ptr = ptr->nexttcb;
        }
    }
    preptr->nexttcb = task;
    task->nexttcb = NULL;
    // ptr = rdqueuehead.nexttcb;
    // if(ptr->para->exeTime != 0){
    //     for(;ptr;ptr = ptr->nexttcb){//debug
    //         myPrintk(0x7,"%d",ptr->para->exeTime);
    //     }
    //     myPrintk(0x7," finishedeq\n");//debug
    // }
    enable_interrupt();
}

myTCB* dq_rr(void){
    if(!rdqueuehead.nexttcb)
        return NULL;

    disable_interrupt();//出队保护
    myTCB* ptr = rdqueuehead.nexttcb;
    if(rdqueuehead.nexttcb->nexttcb == NULL)
        rdqueuehead.nexttcb = NULL;
    else
        rdqueuehead.nexttcb = rdqueuehead.nexttcb->nexttcb;
    enable_interrupt();

    return ptr;
}

void schedulebyRR(void){
    while(1){
        myTCB* tsk = rdqueuehead.nexttcb;
        if(rdqueuehead.nexttcb == NULL){//没有任务待执行则执行idle任务
            if(idletcb==NULL){//idle已被摧毁则重新创建并launch
                int idletid = createTsk(idleTskBdy);
                idletcb = tcb_pool[idletid];
                setTskPara(EXETIME, 0, idletcb->para);
                launchtsk(idletcb,0);
                return ;
            }
            else tsk = idletcb;
        }
        else dq_rr();
        // myPrintk(0x7,"heresche0\n"); 
        if(tsk->id == idletcb->id){
            if(runningtcb){
                if(runningtcb->id == idletcb->id)
                    return ;//only idle task remain, return to restart idle
            }
        }
        
        if(runningtcb == idletcb) {idletcb = NULL;}//idle将被摧毁，置其指示指针为NULL
        if(runningtcb) {
            if(runningtcb == idletcb | runningtcb->runtime >= runningtcb->para->exeTime){
                destroyTsk(runningtcb->id);
                myPrintf(0x7, "=================Finish %d=================\n",runningtcb->id);
                myPrintk(0x7,"                 arrtime: %d\n",runningtcb->para->arrTime);
                myPrintk(0x7, "           runtime/exetime: %d / %d\n\n",runningtcb->runtime,runningtcb->para->exeTime);
            }
            else {
                runningtcb->state = READY;//未执行完挂起，并重新进入就绪队列
                eq_rr(runningtcb);
                myPrintk(0x7,"          -----hanging up %d-----\n",runningtcb->id);
                myPrintk(0x7,"          runtime/exetime: %d / %d\n",runningtcb->runtime,runningtcb->para->exeTime);
                myPrintk(0x7,"          ----------------------\n\n");
            }
        }//摧毁当前任务（基于抢占式，调度时判断任务是否已执行完）

        runningtcb = tsk;
        counter = 0;//重置时间片计时为0
        // myTCB* p = rdqueuehead.nexttcb;
        // myPrintk(0x7,"%d %d\n",(int)p,(int)(p->nexttcb));
        // myPrintk(0x7,"%d, %d\n",tsk->id,tsk->para->arrTime);
        myPrintk(0x7,"          ------running %d-------\n",tsk->id);
        myPrintk(0x7,"          runtime/exetime: %d / %d\n",runningtcb->runtime,runningtcb->para->exeTime);
        myPrintk(0x7,"          ----------------------\n\n");
        tsk->state = RUNNING;
        // if(beforetcb){
        //     context_switch(&(beforetcb->tskptr),tsk->tskptr);
        // }
        // else 
        context_switch(&BspContext,tsk->tskptr);
    }
}

void rr_tickhook(void){
    counter++;
    if(runningtcb->id == idletcb->id) return ;
    runningtcb->runtime++;
    // myPrintk(0x7,"here0\n");
    if(counter == timeslice){
        // myPrintk(0x7,"here1\n");
        counter = 0;
        context_switch(&(runningtcb->tskptr),BspContext);
    }
}

void rr_tskendhook(void){
    myPrintf(0x7, "=================Finish %d=================\n",runningtcb->id);
    myPrintk(0x7,"                 arrtime: %d\n",runningtcb->para->arrTime);
    myPrintk(0x7, "           runtime/exetime: %d / %d\n\n",runningtcb->runtime,runningtcb->para->exeTime);
    // myPrintk(0x7,"----------------------\n\n"); 
}

void rr_createTsk(myTCB* created){
    return ;
}