#ifndef __Timer_H__
#define __Timer_H__

#include "Globals.h"
#include "SDL/SDL.h"
#include <vector>

class Timer
{

public:
	Timer() = default;
	~Timer() = default;

	void StartTimer();
	float ReadTimer() const;
	float StopTimer();


private:
	float initialTime = 0.0f;
	float currentTime;

	bool isActivated = false;
	
};




#endif __Timer_H__
