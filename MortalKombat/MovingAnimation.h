#ifndef MovingAnimation_HEADER
#define MovingAnimation_HEADER

#include "Animation.h"

class MovingAnimation :public Animation {
	int 	dx, dy;
	delay_t		delay;
	bool		continuous;
public:
	int GetDx(void) const;
	int GetDy(void) const;
	void SetDx(offset_t v);
	void SetDy(offset_t v);
	delay_t GetDelay(void) const;
	void SetDelay(delay_t v);
	bool GetContinuous(void) const;
	void SetContinuous(bool v);
	Animation* Clone(animid_t newId)const;
	MovingAnimation(
		int  _dx, int  _dy, delay_t _delay, bool c, animid_t _id
	) : dx(_dx), dy(_dy), delay(_delay), continuous(c), Animation(_id) {}
};

#endif