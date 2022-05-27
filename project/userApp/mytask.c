#include "shell.h"
#include "memTestCase.h"
#include "../myOS/userInterface.h"

void MyTask1(void) {
	myPrintf(0x7, "==================Task 1==================\n");
	myPrintf(0x7, "                Hello MyOS!               \n");
	myPrintf(0x7,"\n");
}

void MyTask2(void) {//in this task we set ourclock
	myPrintf(0x7, "==================Task 2==================\n");
	myPrintf(0x7, "                Get Clock!                \n");
    
    int h,m,s;
    h=0;m=0;s=0;
    getWallClock(&h,&m,&s);
    myPrintf(0x07,"%02d:%02d:%02d\n",h,m,s);
	myPrintf(0x7,"\n");
}

void MyTask3(void) {//in this task we show the memory we have
	myPrintf(0x7, "==================Task 3==================\n");
	myPrintf(0x7, "               Show Memory!               \n");

    myPrintf(0x7,"Kernel memory:\n");
	showpartofEMB(kpMemHandler);
	myPrintf(0x7,"User memory:\n");
	showpartofEMB(pMemHandler);

	myPrintf(0x7,"\n");
}

void MyTask4(void) {
	myPrintf(0x7, "==================Task 4==================\n");
	myPrintf(0x7, "                Do Nothing   		   \n\n");
}

void ShellTask(void) {//task for starting the shell
	myPrintf(0x7, "================Shell Task================\n");
	myPrintf(0x7, "              starting shell...           \n");

    initShell();
    memTestCaseInit();
    startShell();
}