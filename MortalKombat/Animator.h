#ifndef Animator_H
#define Animator_H

#include "MyLibraries.h"
#include <functional>

class Animator {
protected:
	timestamp_t				lastTime;
	animatorstate_t			state;
	std::function<void()>	onFinish;//Lambda Function
	void*					finishClosure;
	void NotifyStopped(void);
public:
	void Stop(void);
	animatorstate_t GetState(void)const;
	bool HasFinished(void) const;
	virtual void TimeShift(timestamp_t offset);//Might Not be usefull
	virtual void Progress(timestamp_t currTime) = 0;

	void SetOnFinish(const std::function<void()>& f, void *c = (void*)0) {
		onFinish = f;
		finishClosure = c;//also this needs fix
	};
	Animator(void);
	virtual ~Animator() {};
};

#endif 