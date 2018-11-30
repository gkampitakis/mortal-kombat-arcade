#include <MyLibraries.h>

class Animator {
public:
	typedef void(*FinishCallback)(Animator*, void*);
protected:
	timestamp_t lastTime;
	animatorstate_t state;
	FinishCallback onFinish;
	void* finishClosure;
	void NotifyStopped(void);
public:
	void Stop(void);
	animatorstate_t GetState(void)const;
	bool HasFinished(void) const;
	virtual void TimeShift(timestamp_t offset);
	virtual void Progress(timestamp_t currTime) = 0;
	void SetOnFinish(FinishCallback f, void *c = (void*)0) {
		onFinish = f;
		finishClosure = c;
	};
	Animator(void);
	virtual ~Animator() {};
};