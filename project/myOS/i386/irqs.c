#include "myPrintk.h"
#include "vga.h" 
void ignoreIntBody(void){
    myPrintk(0x07,"Unknown interrupt\n\0");
    char str[] = "Unknown interrupt";
    for (int i = 0;str[i]!='\0';i++){
        put_char2pos(str[i],0x07,80*25-17+i);
    }
    return;
}
