#include "include/i8253.h"
#include "include/i8259.h"
#include "include/irq.h"
#include "include/uart.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/task.h"
#include "include/myPrintk.h"

extern void myMain(void);		//TODO: to be generalized

void pressAnyKeyToStart(void){
	myPrintk(0x5,"Prepare uart device\n");
	myPrintk(0x5,"Then, press any key to start ...\n");

	uart_get_char();
}

void osStart(void){
	pressAnyKeyToStart(); // prepare for uart device
	init8259A();
	init8253();
	enable_interrupt();

	clear_screen();

	MemInit();
	kMemInit();
	pMemInit();  //after this, we can use kmalloc/kfree and malloc/free

	{
		dPartitionWalkByAddr(kpMemHandler);
		unsigned long tmp = dPartitionAlloc(kpMemHandler,100);
		dPartitionWalkByAddr(kpMemHandler);
		dPartitionFree(kpMemHandler,tmp);
		dPartitionWalkByAddr(kpMemHandler);
	}

	// finished kernel init
	// NOW, run userApp
	// while(1);//debug
	myPrintk(0x2,"START RUNNING......\n");	
	iniTskManager();
	myPrintk(0x2, "STOP RUNNING......ShutDown\n");
	while(1);
}
