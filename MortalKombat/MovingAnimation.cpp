#include "MovingAnimation.h"

offset_t MovingAnimation::GetDx(void) const {
	return dx;
};


offset_t MovingAnimation::GetDy(void) const {
	return dy;
};

void MovingAnimation::SetDx(offset_t v) {
	dx = v;
};


void MovingAnimation::SetDy(offset_t v) {
	dy = v;
};

delay_t MovingAnimation::GetDelay(void) const {
	return delay;
};

void MovingAnimation::SetDelay(delay_t v) {
	delay = v;
};

bool MovingAnimation::GetContinuous(void) const {
	return continuous;
};

void MovingAnimation::SetContinuous(bool v) {
	continuous = v;
};

Animation* MovingAnimation::Clone(animid_t newId) const {
	return new MovingAnimation(dx, dy, delay, continuous, newId);
};