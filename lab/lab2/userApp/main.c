#include "userInterface.h"

void myMain(void) {
    
    int i;  
    for (i = 1; i < 30; i++) 
        myPrintf(i, "%d\n", i);
    myPrintk(0x7, "PB20020586_yza\n"); // 你的学号、姓名

    return;
}
