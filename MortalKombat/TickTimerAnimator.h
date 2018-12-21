#ifndef TickTimerAnimator_HEADER
#define TickTimerAnimator_HEADER

#include "Animator.h"
#include "TickTimerAnimation.h"

class TickTimerAnimator :public Animator {
	TickTimerAnimation *anim;
public:
	void Start(timestamp_t time);//Prolly time and the animation
	void Progress(timestamp_t currTime);
	TickTimerAnimator(TickTimerAnimation* anim);
	~TickTimerAnimator() { delete anim; };
};

#endif