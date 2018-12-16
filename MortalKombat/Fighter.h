#include "MyLibraries.h"
#include "Sprite.h"
#include "TickTimerAnimator.h"
#include "InputContoller.h"
#include "StateTransitions.h"

class Fighter final {
private:
	Sprite*			sprite = nullptr;
	string					nextAction;
	TickTimerAnimator*		tickAnimator = nullptr;	// deferred firing actions; always dynamic
	TickTimerAnimation		tickAnim;
	string					name;
	input::InputController		inputController;
	logic::StateTransitions		stateTransitions;
public:
	// TODO: 
};