#ifndef __TASKCLOCK_H__
#define __TASKCLOCK_H__

void addTaskClockHook(void (*func)(void));
void taskCheck(void);
void runtskhook(void);

#endif