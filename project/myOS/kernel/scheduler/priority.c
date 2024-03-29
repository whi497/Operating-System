#include "../../include/priority.h"
#include "../../include/myPrintk.h"
#include "../../include/irq.h"
#include "../../include/task_arr.h"

struct scheduler sche_PRIO = {
    SCHEDULER_PRIORITY1,
    NULL,
    eqbypriority,
    dqbypriority,
    pri_initscheduler,
    schedulebypriority,
    pri_createTsk,
    NULL,
    pri_tickhook,
};

void pri_initscheduler(void){
    rdqueuehead.nexttcb = NULL;
}

void eqbypriority(myTCB* task){
    myTCB* ptr = rdqueuehead.nexttcb;
    myTCB* preptr = &rdqueuehead;

    disable_interrupt();//入队保护
    // myPrintk(0x7, "eq priority = %d\n", task->para->priority);//debug
    if(!rdqueuehead.nexttcb){
        rdqueuehead.nexttcb = task;
        task->nexttcb = NULL;
    }
    else{
        for(;ptr;ptr=ptr->nexttcb){
            if(task->para->priority > ptr->para->priority){
                preptr->nexttcb = task;
                task->nexttcb = ptr;
                break;
            }
            // if(ptr->nextTID 
            preptr = ptr;
        }
        if(!ptr){
            preptr->nexttcb = task;
            task->nexttcb = NULL;
        }
    }
    // ptr = rdqueuehead.nexttcb;
    // if(ptr->para->priority != 0){
    //     for(;ptr;ptr = ptr->nexttcb){//debug
    //         myPrintk(0x7,"%d",ptr->para->priority);
    //     }
    //     myPrintk(0x7,"\n");
    //     myPrintk(0x7,"finishedeq\n");//debug
    // }
    enable_interrupt();
}

myTCB* dqbypriority(void){
    if(!rdqueuehead.nexttcb)
        return NULL;

    disable_interrupt();//出队保护
    myTCB* ptr = rdqueuehead.nexttcb;
    if(rdqueuehead.nexttcb->nexttcb == NULL)
        rdqueuehead.nexttcb = NULL;
    else
        rdqueuehead.nexttcb = rdqueuehead.nexttcb->nexttcb;
    // myTCB* p = rdqueuehead.nexttcb;
    // if(p){
    //     for(;p;p = p->nexttcb){
    //         myPrintk(0x7,"%d",p->para->priority);
    //     } 
    //     myPrintk(0x7,"\n");
    //     myPrintk(0x7,"finisheddq\n");//debug
    // }
    enable_interrupt();

    return ptr;
}

void schedulebypriority(void){
    // if(p!=NULL){
    //     for(;p;p = p->nexttcb){
    //         myPrintk(0x7,"%d",p->para->priority);
    //     } 
    //     myPrintk(0x7,"\n");
    // }
    // while(1){
    myTCB* tsk = rdqueuehead.nexttcb;
    if(rdqueuehead.nexttcb == NULL){//没有任务待执行则执行idle任务
        if(idletcb==NULL){//idle已被摧毁则重新创建并launch
            int idletid = createTsk(idleTskBdy);
            idletcb = tcb_pool[idletid];
            setTskPara(PRIORITY, 0, idletcb->para);
            launchtsk(idletcb,0);
            return ;
        }
        else tsk = idletcb;
        
    }
    else dqbypriority();
    // myPrintk(0x7,"heresche0\n"); 
    if(tsk->id == idletcb->id){
        if(runningtcb){
            if(runningtcb->id == idletcb->id)
                return ;//only idle task remain, return to restart idle
        }
    }
    
    if(runningtcb == idletcb) {idletcb = NULL;}//idle将被摧毁，置其指示指针为NULL
    if(runningtcb) {destroyTsk(runningtcb->id);}//摧毁当前任务（基于非抢占式，调度时认为任务已完成）
    runningtcb = tsk;
    // myTCB* p = rdqueuehead.nexttcb;
    // myPrintk(0x7,"%d %d\n",(int)p,(int)(p->nexttcb));
    // myPrintk(0x7,"%d, %d\n",tsk->id,tsk->para->arrTime);
    tsk->state = RUNNING;

    context_switch(&BspContext,tsk->tskptr);
    // }
}

void pri_createTsk(myTCB* created){
    return ;
}

void pri_tickhook(void){
    return ;
}