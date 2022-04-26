#include "vga.h"
#include "vsprintf.h"
#include <stdarg.h>
char Buf[400];

void (*wallClock_hook)(int, int, int) = 0;

void oneTickUpdateWallClock(int HH, int MM, int SS){//调用 wallClock_hook 函数
	if(wallClock_hook) wallClock_hook(HH,MM,SS);
} 

void setWallClockHook(void (*func)(int, int, int)) {//设置 wallClock_hook 的值
	wallClock_hook = func;
}

void setWallClock(int HH,int MM,int SS){
	//todo//done
	int args[3] = { HH, MM, SS };
	char* format = "%02d:%02d:%02d";
    vsprintf(Buf, format, args);
	int len = strlen(Buf);
	for (int i = 0;Buf[i]!='\0';i++){
        put_char2pos(Buf[i],0x07,80*25-len+i);
    }
}

void getWallClock(int *HH,int *MM,int *SS){
	//todo
	unsigned short int* p = VGA_BASE+(80*25-8)*2;
	*HH = ((*p & 0x00ff)-48)*10+((*(p+1) & 0x00ff)-48);
	p+=3;
	*MM = ((*p & 0x00ff)-48)*10+((*(p+1) & 0x00ff)-48);
	p+=3;
	*SS = ((*p & 0x00ff)-48)*10+((*(p+1) & 0x00ff)-48);
}