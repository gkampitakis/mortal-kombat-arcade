#include "MyLibraries.h"
#include "Animator.h"

class AnimatorHolder {
	static list<Animator*> running, suspended;
	class ProgressFunctor : public unary_function<Animator*, void> {
		timestamp_t t;
	public:
		void operator()(Animator* a) const { a->Progress(t); }
		ProgressFunctor(timestamp_t _t) : t(_t) {}
	};
public:
	AnimatorHolder(void) {}

	static void Register(Animator* a); 
	static void Cancel(Animator* a);
	static void MarkAsRunning(Animator* a);
	static void MarkAsSuspended(Animator* a);
	static void Progress(timestamp_t currTime);
};