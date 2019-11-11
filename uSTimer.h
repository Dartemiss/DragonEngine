#ifndef __uSTimer_H__
#define __uSTimer_H__
class uSTimer
{
public:
	uSTimer() = default;
	~uSTimer() = default;

	void StartTimer();
	void Update();
	float ReadTimer();
	float StopTimer();
	


private:
	
	float currentTime;
	float deltaTime;
	float initialTime;

};
#endif __uSTimer_H__
