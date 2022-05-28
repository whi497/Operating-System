#include "../../include/sjf.h"
#include "../../include/myPrintk.h"
#include "../../include/irq.h"
#include "../../include/task_arr.h"

struct scheduler sche_SJF = {
    SCHEDULER_SJF,
    NULL,
    eq_sjf,
    dq_sjf,
    sjf_initscheduler,
    schedulebysjf,
    sjf_createTsk,
    NULL,
    NULL
};

void sjf_initscheduler(void){
    rdqueuehead.nexttcb = NULL;
}

void eq_sjf(myTCB* task){
    myTCB* ptr = rdqueuehead.nexttcb;
    myTCB* preptr = &rdqueuehead;

    disable_interrupt();//入队保护
    if(!rdqueuehead.nexttcb){
        rdqueuehead.nexttcb = task;
        task->nexttcb = NULL;
    }
    else{
        for(;ptr;ptr=ptr->nexttcb){
            if(task->para->exeTime < ptr->para->exeTime){
                preptr->nexttcb = task;
                task->nexttcb = ptr;
                break;
            }
            preptr = ptr;
        }
        if(!ptr){
            preptr->nexttcb = task;
            task->nexttcb = NULL;
        }
    }
    enable_interrupt();
}

myTCB* dq_sjf(void){
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

void schedulebysjf(void){

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
    else dqbypriority();
    if(tsk->id == idletcb->id){
        if(runningtcb){
            if(runningtcb->id == idletcb->id)
                return ;//only idle task remain, return to restart idle
        }
    }
    
    if(runningtcb == idletcb) {idletcb = NULL;}//idle将被摧毁，置其指示指针为NULL
    if(runningtcb) {destroyTsk(runningtcb->id);}//摧毁当前任务（基于非抢占式，调度时认为任务已完成）
    runningtcb = tsk;
    // myTCB* "%d, %d\n",tsk->id,tsk->para->arrTime);
    tsk->state = RUNNING;
    myPrintk(0x7,"          ------running %d-------\n",tsk->id);
    myPrintk(0x7,"               exetime: %d\n",runningtcb->para->exeTime);
    myPrintk(0x7,"          ----------------------\n\n");
    context_switch(&BspContext,tsk->tskptr);
    // }
}

void sjf_createTsk(myTCB* created){
    return ;
}

void sjf_tickhook(void){
    return ;
}