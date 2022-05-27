#include "usertask.h"

void RRTsk1(void){
    MyTask1();
    wait(6);
    myPrintf(0x7, "===============Finish Task1===============\n");
}
void RRTsk2(void){
    MyTask2();
    wait(5);
    myPrintf(0x7, "===============Finish Task2===============\n");
}
void RRTsk3(void){
    MyTask3();
    wait(4);
    myPrintf(0x7, "===============Finish Task3===============\n");
}
void RRTsk4(void){
    MyTask4();
    wait(3);
    myPrintf(0x7, "===============Finish Task4===============\n");
}

void test_rr_sche(void){
    int tsk1 = createTsk(RRTsk1);
    int tsk2 = createTsk(RRTsk2);
    int tsk3 = createTsk(RRTsk3);
    int tsk4 = createTsk(RRTsk4);
    // int shelltsk = createTsk(ShellTask);
    setTskPara(EXETIME,6,tcb_pool[tsk1]->para);
    setTskPara(EXETIME,5,tcb_pool[tsk2]->para);
    setTskPara(EXETIME,4,tcb_pool[tsk3]->para);
    setTskPara(EXETIME,3,tcb_pool[tsk4]->para);
    // setTskPara(EXETIME,1,tcb_pool[shelltsk]->para);
    launchtsk(tcb_pool[tsk1],3);
    launchtsk(tcb_pool[tsk2],3);
    launchtsk(tcb_pool[tsk3],3);
    launchtsk(tcb_pool[tsk4],3);
    // launchtsk(tcb_pool[shelltsk],4);
}