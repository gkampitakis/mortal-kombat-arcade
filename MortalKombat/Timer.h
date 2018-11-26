#include <MyLibraries.h>

class Timer {
public:
	Timer();
	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();
	
	bool isStarted();
	bool isPaused();
	string ReverseTimer(int t);
private:
	Uint32 StartTicks;
	Uint32 PausedTicks;

	bool Paused;
	bool Started;
};