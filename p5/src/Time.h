#pragma once

#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

static class Time
{
public:
	Time(float startTime);
	~Time();
	float deltaTime;
	void updateTime(float curTime);

private:
	float curTime;
	float prevTime;
	float startTime;

};

#endif