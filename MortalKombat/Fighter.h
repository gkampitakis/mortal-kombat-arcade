#include "MyLibraries.h"
#include "Sprite.h"
#include "TickTimerAnimator.h"
#include "TickTimerAnimation.h"
#include "InputContoller.h"
#include "StateTransitions.h"

class Fighter final {
private:
	Sprite*			sprite;
	string					nextAction;
	TickTimerAnimator*		tickAnimator;	// deferred firing actions; always dynamic
	TickTimerAnimation		tickAnim;
	string					name;
	input::InputController		inputController;
	logic::StateTransitions		stateTransitions;
	//state
public:
	// TODO: 
	Fighter(string Name);
	~Fighter() {};
	void setStateMachine();
	void Handler();
	void Draw(SDL_Surface& gScreenSurface,string name,Rect t);//test function
	bool initialize(const string& path);//give json as argument
};