#include "Animation.h"

class TickTimerAnimation : public Animation {
public:
	typedef void(*TickFunc)(void* closure);
private:
	delay_t delay;
	byte repetitions;
	TickFunc action;
	void* closure;
public:
	TickTimerAnimation(animid_t id) :
		Animation(id),
		delay(0),
		repetitions(1),
		action((TickFunc)0), closure((void*)0) {}
};