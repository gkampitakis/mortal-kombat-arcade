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

	static void Register(Animator* a) { suspended.push_back(a); };
	static void Cancel(Animator* a) { suspended.remove(a); };
	static void MarkAsRunning(Animator* a)
	{
		suspended.remove(a); running.push_back(a);
	};
	static void MarkAsSuspended(Animator* a)
	{
		running.remove(a); suspended.push_back(a);
	};
	static void Progress(timestamp_t currTime) {
		timestamp_t tmpTime = SDL_GetTicks();

		auto tmp = running;//if i call mark suspended throws error if i touch the iterating loop
		for (list<Animator*>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
			if ((*it)->GetState() == ANIMATOR_RUNNING) {
				(ProgressFunctor(tmpTime)(*(it)));//check if this works 
			}
			else {
				MarkAsSuspended(*it);
			}

		}
	};
};