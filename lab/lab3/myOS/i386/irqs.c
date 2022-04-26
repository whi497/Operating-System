#include "myPrintk.h"
#include "vga.h" 
void ignoreIntBody(void){
    myPrintk(0x09,"\nUnknown interrupt\n\0");
    char str[] = "Unknown interrupt";
    for (int i = 0;str[i]!='\0';i++){
        put_char2pos(str[i],0x07,80*24+i);
    }
    return;
    // put_char2pos("Unknown interrupt1\0",0x4,24,0);
}
