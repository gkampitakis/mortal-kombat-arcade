#include "Animator.h"
#include "TickTimerAnimation.h"

class TickTimerAnimator :public Animator {
	TickTimerAnimation *anim;
public:
	void Start();//Sprite Prolly time and the animation
	void Progress(timestamp_t currTime);
	TickTimerAnimator(TickTimerAnimation *tick);
	~TickTimerAnimator() {};
};