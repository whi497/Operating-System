#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"
#include "usertask.h"

void wallClock_hook_main(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
	sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x7E,24,72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
    	setWallClockHook(&wallClock_hook_main);
}

void myMain(void){    
    clear_screen();
	myPrintf(0x7, "===============Initial Task===============\n");
	myPrintf(0x7, "            Initial user task...        \n\n");

    doSomeTestBefore();

	switch(schepolicy){ 
        case SCHEDULER_FCFS: break; 
        case SCHEDULER_SJF: break; 
        case SCHEDULER_PRIORITY0: { 
			test_prio_sche();
            break; 
        } 
        case SCHEDULER_RR: { 
			test_rr_sche();
            break; 
        } 
        default: break; 
    }
	myPrintf(0x7,"waiting...\n");
	wait(10);
	myPrintf(0x7,"wait finish\n");	
	// myTCB* ptr = rdqueuehead.nexttcb;
	// for(;ptr;ptr = ptr->nexttcb){//debug
	// 	myPrintf(0x7,"%d",ptr->para->priority);
	// }
	// myPrintf(0x7,"\n");
	while(1);
    return;
}
