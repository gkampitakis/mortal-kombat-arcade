#include "TickTimerAnimation.h"

delay_t TickTimerAnimation::GetDelay(void) const {
	return delay;
};

void TickTimerAnimation::Run(void) {
	if(action) action();
};

byte TickTimerAnimation::GetReps(void) const {
	return repetitions;
};

TickTimerAnimation& TickTimerAnimation::SetReps(byte reps) {
	repetitions = reps;
	return *this;
}

TickTimerAnimation& TickTimerAnimation::SetDelay(delay_t d) {
	delay = d;
	return *this;
};