#include "TickTimerAnimator.h"

TickTimerAnimator::TickTimerAnimator() {};

void TickTimerAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		anim->Run();
		lastTime += anim->GetDelay();

		if (anim->GetReps() != 0 && anim->GetReps() == 1) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
		}
		else {
			
			anim->SetReps(anim->GetReps() - 1);
		}
	}
};

void TickTimerAnimator::Start(TickTimerAnimation* anim, timestamp_t time) {
	TickTimerAnimator::anim = anim;
	lastTime = time;
	state = ANIMATOR_RUNNING;
};
