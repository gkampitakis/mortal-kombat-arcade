#include "FrameRangeAnimator.h"

void FrameRangeAnimator::Start(Sprite* s, FrameRangeAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
	sprite->SetFrame(currFrame = anim->GetStartFrame());
};

void FrameRangeAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		if (currFrame == anim->GetEndFrame()) {
			currFrame = anim->GetStartFrame();
		}
		else ++currFrame;

		Point x = { anim->GetDx(),anim->GetDy() };
		sprite->Move(x);
		sprite->SetFrame(currFrame);

		lastTime += anim->GetDelay();
		if (currFrame == anim->GetEndFrame() && !anim->GetContinuous()) {//The lest rep is the 1
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
};