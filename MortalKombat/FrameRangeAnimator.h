#include "MyLibraries.h"
#include "Animator.h"
#include "Sprite.h"
#include "FrameRangeAnimation.h"

class FrameRangeAnimator :public Animator {
	Sprite*					sprite;
	FrameRangeAnimation*	anim;
	frame_t					currFrame;
public:
	void Progress(timestamp_t currTime);
	void Start(Sprite* s, FrameRangeAnimation* a, timestamp_t t);

	FrameRangeAnimator(void) :
		sprite((Sprite*)0),
		anim((FrameRangeAnimation*)0), currFrame(0xFF) {}

	~FrameRangeAnimator() {}
};