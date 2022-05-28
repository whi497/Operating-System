#include "../include/myPrintk.h"
#include "../include/task.h"
#include "../include/task_arr.h"
#include "../include/irq.h"
#define HOOK_LIST_SUM 10
#define NULL (void*)0

int list_index = 0;

extern int tick_number;

void (* tskclklist[HOOK_LIST_SUM])(void) = {NULL};

void addTaskClockHook(void (*func)(void)){
    if(list_index >+ HOOK_LIST_SUM){
        myPrintk(0x3,"ERROR: too many task clock hooks!\n");
        return ;
    }
    tskclklist[list_index] = func;
    list_index++;
    return ;
}

void taskCheck(void) {
    int seconds = tick_number/100;
    myTCB* ptr = arrqueue.nexttcb;
    // myTCB* p = arrqueue.nexttcb;
    // for(;p;p = p->nexttcb){//debug
    //     myPrintk(0x7,"%d ",p->para->arrTime);
    // }
    // myPrintk(0x7, "\n");
    // myPrintk(0x7,"arrhead = %d\n",ptr->para->arrTime);
    if(!ptr) return ;
    // disable_interrupt();
    while(ptr) {
        myTCB* tsktcb = NULL;
        // myPrintk(0x7,"arrtime:%d,seconds:%d\n",ptr->para->arrTime,seconds);
        if(ptr->para->arrTime <= seconds){
            // myPrintk(0x7,"start:%d next:%d\n",ptr->para->arrTime,ptr->nexttcb->para->arrTime);
            if(arrqueue.nexttcb->nexttcb == NULL)
                arrqueue.nexttcb = NULL;
            else
                arrqueue.nexttcb = arrqueue.nexttcb->nexttcb;
            tsktcb = ptr;
            ptr = ptr->nexttcb;
            // myPrintk(0x7,"%d ",arrqueue.nexttcb->para->arrTime);
            tskStart(tsktcb);
            // myPrintk(0x7,"%d\n",arrqueue.nexttcb->para->arrTime);
            // myTCB* p = arrqueue.nexttcb;
            // for(;p;p = p->nexttcb){//debug
            //     myPrintk(0x7,"%d ",p->para->arrTime);
            // }
            // myPrintk(0x7, "\n");
        }
        else break;
    }
    // enable_interrupt();
}

void runtskhook(void){
    for(int i=0;i<HOOK_LIST_SUM;i++){
        // myPrintk(0x7,"here task hook\n");
        if(tskclklist[i])tskclklist[i]();
        else break;
    }
}