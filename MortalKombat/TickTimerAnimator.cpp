#include "TickTimerAnimator.h"

TickTimerAnimator::TickTimerAnimator(TickTimerAnimation* anim) {
	TickTimerAnimator::anim = anim;
};

void TickTimerAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		anim->Run();
		lastTime += anim->GetDelay();

		if (anim->GetReps() != 0 && anim->GetReps() == 1) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();//This one here fix 
		}
		else {
			anim->SetReps(anim->GetReps() - 1);
		}
	}
};

void TickTimerAnimator::Start(timestamp_t time) {
	lastTime = time;
	state = ANIMATOR_RUNNING;
};
