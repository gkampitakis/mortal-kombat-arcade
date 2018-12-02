#include "AnimatorHolder.h"

list<Animator *> AnimatorHolder::running, AnimatorHolder::suspended;

void AnimatorHolder::Progress(timestamp_t currTime) {
	timestamp_t tmpTime = SDL_GetTicks();

	auto tmp = running;//if i call mark suspended throws error if i touch the iterating loop
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

void AnimatorHolder::MarkAsSuspended(Animator* a) {
	running.remove(a); suspended.push_back(a);
	cout << "Finished\n";//Too see when tha animation is suspended at terminal
};

//one function to be called in its circle to  delete the suspended animators
//in the suspended list and finished state delete them

void AnimatorHolder::CleanUp(void) {
	for (auto&& it : running) {
		delete it;
	}
	running.clear();

	for (auto&& it : suspended) {
		delete it;
	}
	suspended.clear();
};