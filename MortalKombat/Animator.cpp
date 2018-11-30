#include "Animator.h"

void Animator::Stop(void) {
	if (!HasFinished()) {
		state = ANIMATOR_STOPPED;
		NotifyStopped();
	}
};

bool Animator::HasFinished(void) const {
	return state != ANIMATOR_RUNNING;
};

void Animator::NotifyStopped(void) {
	if (onFinish) (*onFinish)(this, finishClosure);
};

void Animator::TimeShift(timestamp_t offset) {
	lastTime += offset;
};

Animator::Animator(void) :lastTime(0), state(ANIMATOR_FINISHED),
onFinish((FinishCallback)0), finishClosure((void*)0) {};

animatorstate_t Animator::GetState(void) const {
	return state;
}