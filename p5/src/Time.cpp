#include "Time.h"


Time::Time(float startTime)
{
	this->startTime = startTime;
	curTime = 0;
	deltaTime = 0;
	prevTime = 0;
}


Time::~Time()
{
}

void Time::updateTime(float curTime)
{
	float actualCurTime = curTime - startTime;
	prevTime = this->curTime;
	this->curTime = actualCurTime;
	deltaTime = actualCurTime - prevTime;
}

