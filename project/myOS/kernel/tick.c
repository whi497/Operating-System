#include "wallClock.h"
int system_ticks;
int HH,MM,SS;

void tick(void){
	system_ticks++;
	//todo
	setWallClock(HH,MM,SS);
	return;
}

