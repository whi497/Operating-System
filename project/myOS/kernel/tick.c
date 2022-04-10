#include "wallClock.h"
long int system_ticks;
int HH,MM,SS;

void tick(void){
	system_ticks++;
	//todo//done
	int MS;
	MS = (10*system_ticks)%1000;
	SS = ((10*system_ticks)/1000)%60;
	MM = (((10*system_ticks)/1000)/60)%60;
	HH = ((((10*system_ticks)/1000)/60)/60)%24;
	oneTickUpdateWallClock(HH, MM, SS);
	return;
}

