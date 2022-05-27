#include "usertask.h"

void test_prio_sche(void){
    int tsk1 = createTsk(MyTask1);
    int tsk2 = createTsk(MyTask2);
    int tsk3 = createTsk(MyTask3);
    // int shelltsk = createTsk(ShellTask);

    setTskPara(PRIORITY,3,tcb_pool[tsk1]->para);
    setTskPara(PRIORITY,4,tcb_pool[tsk2]->para);
    setTskPara(PRIORITY,3,tcb_pool[tsk3]->para);
    // setTskPara(PRIORITY,2,tcb_pool[shelltsk]->para);

    tskStart(tcb_pool[tsk1]);
    tskStart(tcb_pool[tsk2]);
    tskStart(tcb_pool[tsk3]);
    // tskStart(tcb_pool[shelltsk]);
}
