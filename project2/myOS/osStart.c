#include "vga.h"
#include "myPrintk.h"
// extern int start;//check the address
// extern int _en;
// extern int _bss_start;
// extern int __bss_end;
// extern int _end;
/* 此文件无需修改 */

// 用户程序入口
void myMain(void);

void osStart(void) {
    clear_screen();
    myPrintk(0x2, "Starting the OS...\n");
    int address[5] = {0};
    // address[0] = &start;//check the address
    // address[1] = &_en;
    // address[2] = &_bss_start;
    // address[3] = &__bss_end;
    // address[4] = &_end;
    // for(int i=0;i<5;i++) {
    //     address[i]-=1048576;
    //     myPrintk(0x2, "%d\n",address[i]);
    // }
    // while(1);
    myMain();
    myPrintk(0x2, "Stop running... shutdown\n");
    while(1); 
}
