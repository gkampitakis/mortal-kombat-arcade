#ifndef TickTimerAnimation_H
#define TickTimerAnimation_H

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
		delay(500),//placeholder
		repetitions(0),//placeholder
		action((TickFunc)0), closure((void*)0) {}

	TickTimerAnimation *Clone(animid_t newId) const {
		return new TickTimerAnimation(newId);
	};

	delay_t GetDelay(void) const;
	void Run(void);
	byte GetReps() const;
	void SetReps(byte);
};

#endif