extern void oneTickUpdateWallClock(void);       //TODO: to be generalized
extern void taskCheck(void);     
//#ifndef __TICK_HOOK__
//#define __TICK_HOOK__
void (*tick_hook)(void) = 0; //{};
int check_flag = 0;
//#endif
#include "../include/irq.h"

int tick_number = 0;
void tick(void){
	int temp_col, temp_row;	
	tick_number++;	
	//myPrintk(0x4,"tick::%d\n",tick_number);

	oneTickUpdateWallClock();
	if(check_flag){
		// disable_interrupt();
		// myPrintk(0x7,"tick = %d\n",tick_number);
		// while(1);
		// enable_interrupt();//debug
		taskCheck();
	}
	/* call hooks 
	scheduler_tick();  // RR
	tick_hook_arr();  // arriving	

	if(watchdogTimer_func) watchdogTimer_func(); 
    */
	if(tick_hook) tick_hook();  //user defined   
}
