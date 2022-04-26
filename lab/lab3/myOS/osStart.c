#include "myPrintk.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
void myMain(void);
// extern int start;//check the address
// extern int _en;
// extern int _bss_start;
// extern int _end;

void osStart(void)
{
	init8259A();
	init8253();
	enable_interrupt();
    tick();
    clear_screen();
    
    myPrintk(0x2, "START RUNNING......\n");
    // int address[4] = {0};
    // address[0] = &start;//check the address
    // address[1] = &_en;
    // address[2] = &_bss_start;
    // address[3] = &_end;
    // for(int i=0;i<4;i++) {
    //     address[i]-=1048576;
    //     myPrintk(0x2, "%d\n",address[i]);
    // }
    // while(1);
    myMain();
    myPrintk(0x2, "STOP RUNNING......ShutDown\n");
    while (1);
}
