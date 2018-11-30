#include "Animator.h"
#include "TickTimerAnimation.h"

class TickTimerAnimator :public Animator {
	TickTimerAnimation *anim;
public:
	void Start(TickTimerAnimation* anim,timestamp_t time);//Prolly time and the animation
	void Progress(timestamp_t currTime);
	TickTimerAnimator();
	~TickTimerAnimator() {};
};