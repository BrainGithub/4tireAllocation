#include "myinc.h"

extern volatile uint32_t curTick;

void * emitter_task(void *arg)
{
	int count=0;
	printf("in emitter_task\n");
	while(1)
	{
		sleep(1);
		printf("emitter producing...\n");
		curTick++;

	}
    	return 0;
}
