#include "../../include/priority.h"
#include "../../include/myPrintk.h"
#include "../../include/irq.h"

struct scheduler sche_PRIO = {
    SCHEDULER_PRIORITY0,
    NULL,
    eqbypriority,
    dqbypriority,
    pri_initscheduler,
    schedulebypriority,
    pri_createTsk,
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
    if(rdqueuehead.nexttcb == NULL){
        if(idletcb)
            eqbypriority(idletcb);//取出idle任务需放回
        else{
            int idletid = createTsk(idleTskBdy);
            idletcb = tcb_pool[idletid];
            setTskPara(PRIORITY, 0, idletcb->para);
            launchtsk(idletcb,0);
            myTCB* p = rdqueuehead.nexttcb;
            myPrintk(0x7,"%d %d\n",(int)p,(int)(p->nexttcb));
        }
    }
    // myPrintk(0x7,"heresche0\n"); 
    myTCB* tsk = dqbypriority();
    if(tsk->id == idletcb->id){
        if(runningtcb){
            if(runningtcb->id == idletcb->id)
                return ;//only idle task remain, return to restart schedule
        }
    }
    
    if(runningtcb == idletcb) {idletcb = NULL;myPrintk(0x7,"hereidle\n");}
    if(runningtcb) {destroyTsk(runningtcb->id);myPrintk(0x7,"hereidle\n");}

    // myPrintk(0x7,"heresche1\n");
    runningtcb = tsk;
    myTCB* p = rdqueuehead.nexttcb;
    myPrintk(0x7,"%d %d\n",(int)p,(int)(p->nexttcb));
    myPrintk(0x7,"%d, %d\n",tsk->id,tsk->para->arrTime);
    tsk->state = RUNNING;
    // myPrintk(0x7,"running tsk id = %d\n",tsk->id);
    // while(1);
    if(p){
        for(;p;p = p->nexttcb){
            myPrintk(0x7,"%d",p->para->priority);
        } 
        myPrintk(0x7,"\n");
    }
    context_switch(&BspContext,tsk->tskptr);
}

void pri_createTsk(myTCB* created){
    return ;
}

void pri_tickhook(void){
    return ;
}