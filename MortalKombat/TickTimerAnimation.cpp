#include "TickTimerAnimation.h"

delay_t TickTimerAnimation::GetDelay(void) const {
	return delay;
};

void TickTimerAnimation::Run(void) {
	cout << "Hello World\n";//Place Holder for the callback function
};

byte TickTimerAnimation::GetReps(void) const {
	return repetitions;
};

void TickTimerAnimation::SetReps(byte reps) {
	repetitions = reps;
}
/*
	Here Fix the chained methods
*/