#include <genTimerCounterMarco.h>

void duty_cycle(int freq,int prescaler,float duty,int* dis){
	double t = F_CPU / freq / prescaler;
	int t1 = t * duty;
	int t2 = t * (1. - duty) ;
	dis[0] = 256 - t1;
	dis[1] = 256 - t2;
}
