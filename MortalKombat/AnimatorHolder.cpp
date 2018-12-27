#include "AnimatorHolder.h"

list<Animator *> AnimatorHolder::running, AnimatorHolder::suspended;

void AnimatorHolder::Progress(timestamp_t currTime) {
	timestamp_t tmpTime = SDL_GetTicks();

	auto tmp = running;
	for (list<Animator*>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
		if ((*it)->GetState() == ANIMATOR_RUNNING) {
			(ProgressFunctor(tmpTime)(*(it)));
		}
	}
};

void AnimatorHolder::Register(Animator* a) {
	suspended.push_back(a);
};

void AnimatorHolder::Cancel(Animator* a) {
	suspended.remove(a);
};

void AnimatorHolder::MarkAsRunning(Animator* a) {
	suspended.remove(a); running.push_back(a);
};

void AnimatorHolder::Remove(Animator* a) {
	running.remove(a);
}

void AnimatorHolder::MarkAsSuspended(Animator* a) {
	running.remove(a); suspended.push_back(a);
};

void AnimatorHolder::CleanUp(void) {
	for (auto&& it : running) {
	//	delete it; BUG NEEDS FIX
	}
	running.clear();

	for (auto&& it : suspended) {
		delete it;//Something here crashes
	}
	suspended.clear();
};