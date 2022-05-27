#include "usertask.h"

void test_prio_sche(void){
    int tsk1 = createTsk(MyTask1);
    int tsk2 = createTsk(MyTask2);
    int tsk3 = createTsk(MyTask3);
    int tsk4 = createTsk(MyTask4);
    int shelltsk = createTsk(ShellTask);

    setTskPara(PRIORITY,4,tcb_pool[tsk1]->para);
    setTskPara(PRIORITY,2,tcb_pool[tsk2]->para);
    setTskPara(PRIORITY,3,tcb_pool[tsk3]->para);
    setTskPara(PRIORITY,2,tcb_pool[tsk4]->para);
    setTskPara(PRIORITY,1,tcb_pool[shelltsk]->para);

    launchtsk(tcb_pool[tsk1],0);
    launchtsk(tcb_pool[tsk2],2);
    launchtsk(tcb_pool[tsk3],0);
    launchtsk(tcb_pool[tsk4],2);
    launchtsk(tcb_pool[shelltsk],4);
    // while(1);
}
