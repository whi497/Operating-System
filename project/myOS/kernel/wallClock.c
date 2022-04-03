#include "vga.h"
#include "vsprintf.h"
#include <stdarg.h>
char Buf[400];
void setWallClock(int HH,int MM,int SS){
	//todo//done
	int args[3] = { HH, MM, SS };
	char* format = "%d:%d:%d";
    vsprintf(Buf, format, args);
	int len = strlen(Buf);
	for (int i = 0;Buf[i]!='\0';i++){
        put_char2pos(Buf[i],0x07,80*24-len+i);
    }
}

void getWallClock(int *HH,int *MM,int *SS){
	//todo
}