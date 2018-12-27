#ifndef TickTimerAnimation_H
#define TickTimerAnimation_H

#include "Animation.h"
#include <functional>

class TickTimerAnimation : public Animation {

private:
	delay_t				delay;
	byte				repetitions;
	std::function<void()> action;
	void*				closure;
public:
	TickTimerAnimation(animid_t id) :
		Animation(id),
		delay(3000),
		repetitions(10),
		action((const std::function<void()>)0), closure((void*)0) {
	}

	TickTimerAnimation() {};
	TickTimerAnimation *Clone(animid_t newId) const {
		return new TickTimerAnimation(newId);
	}

	TickTimerAnimation& setOnTick(const std::function<void()>& f, void *c = (void *)0) {
		action = f, closure = c;
		return *this;
	};

	delay_t GetDelay(void) const;
	TickTimerAnimation& SetDelay(delay_t);
	void Run(void);
	byte GetReps() const;
	TickTimerAnimation& SetReps(byte);
};

#endif