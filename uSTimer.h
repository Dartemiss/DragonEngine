#ifndef __uSTimer_H__
#define __uSTimer_H__
class uSTimer
{
public:
	uSTimer() = default;
	~uSTimer() = default;

	void StartTimer();
	float ReadTimer() const;
	float StopTimer();
	


private:
	
	float currentTime;
	float deltaTime;
	float initialTime;

	bool isActivated = false;

};
#endif __uSTimer_H__
