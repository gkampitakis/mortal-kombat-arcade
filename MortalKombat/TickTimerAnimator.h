#include "Animator.h"
//#include "TickTimerAnimation.h"

class TickTimerAnimator :public Animator {
	//TickTimerAnimation *anim;
public:
	void Start();
	void Progress(timestamp_t currTime);
	TickTimerAnimator();
	~TickTimerAnimator() {};
};