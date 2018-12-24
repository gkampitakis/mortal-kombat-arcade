#include "FrameRangeAnimation.h"

frame_t FrameRangeAnimation::GetStartFrame(void) const {
	return start;
};

frame_t FrameRangeAnimation::GetEndFrame(void) const {
	return end;
};

void FrameRangeAnimation::SetEndFrame(frame_t v) {
	end = v;
};

void FrameRangeAnimation::SetStartFrame(frame_t v) {
	start = v;
};

Animation* FrameRangeAnimation::Clone(animid_t newId) const {
	return new FrameRangeAnimation(start, end, GetDx(), GetDy(), GetDelay(), GetContinuous(), newId);
};