#include "myinc.h"
#include <iostream>
#include <queue>

#include "simm_emitter_task.h"

using namespace std;

extern volatile uint32_t curTick;

queue<int16_t> velXQueue;
queue<int16_t> gyroZQueue;

//velocity of acc forward
int16_t velXReadFromEmitter()
{
	int16_t afX = 0;
	if (!velXQueue.empty())
	{
		afX = velXQueue.front();
		velXQueue.pop();
	}
		
	return afX;
}

//gyro of thetaZ
int16_t gyroThetaZReadFromEmitter()
{
	int16_t gyroZ = 0;
	if (!velXQueue.empty())
	{
		gyroZ = gyroZQueue.front();
		gyroZQueue.pop();
	}

	return gyroZ;
}

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
