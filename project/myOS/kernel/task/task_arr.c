#include "../../include/task_arr.h"
#include "../../include/irq.h"

void iniarrqueue(void){
    arrqueue.nexttcb = NULL;
}

void eqbyarrtime(myTCB* task){
    myTCB* ptr = arrqueue.nexttcb;
    myTCB* preptr = &arrqueue;

    disable_interrupt();//入队保护
    if(!arrqueue.nexttcb){
        arrqueue.nexttcb = task;
        task->nexttcb = NULL;
    }
    else{
        for(;ptr;ptr=ptr->nexttcb){
            if(task->para->arrTime < ptr->para->arrTime){
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
    ptr = arrqueue.nexttcb;
    // for(;ptr;ptr = ptr->nexttcb){//debug
	// 	myPrintk(0x7,"%d",ptr->para->arrTime);
	// }
    // myPrintk(0x7, "\n");
    enable_interrupt();
}

void launchtsk(myTCB* task,int arrtime){
    setTskPara(ARRTIME, arrtime, task->para);
    if(arrtime == 0) tskStart(task);
    else eqbyarrtime(task);
}
