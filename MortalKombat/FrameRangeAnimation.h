#include "MyLibraries.h"
#include "MovingAnimation.h"

class FrameRangeAnimation :public MovingAnimation {
	frame_t start, end;
public:
	frame_t GetStartFrame(void) const;
	void SetStartFrame(frame_t v);
	frame_t GetEndFrame(void) const;
	void SetEndFrame(frame_t v);
	Animation* Clone(animid_t newId)const;
	FrameRangeAnimation(
		frame_t s, frame_t e,
		offset_t dx, offset_t dy, delay_t d, bool c, animid_t id
	) : start(s), end(e), MovingAnimation(dx, dy, d, c, id) {}
};