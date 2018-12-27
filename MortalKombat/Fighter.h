#include "MyLibraries.h"
#include "Sprite.h"
#include "TickTimerAnimator.h"
#include "TickTimerAnimation.h"
#include "InputContoller.h"
#include "StateTransitions.h"
#include "FrameRangeAnimator.h"

#define	FIGHTER_ACTION_DELAY_MSECS	150 //<-----------------FOR DEBUGGING

class Fighter final {
	using Input = std::set<std::string>;
private:
	Sprite*					sprite;
	string					nextAction;
	TickTimerAnimator*		tickAnimator;	// deferred firing actions; always dynamic
	TickTimerAnimation		tickAnim;
	FrameRangeAnimator*		animator;
	string					name;
	float					health;
	int						win;
	input::InputController		inputController;
	logic::StateTransitions		stateTransitions;
	
	void SetActionWithAnimator(const std::function<void()>& f);
	const string Make_key(const Input& input) const;
public:
	// TODO: 
	Fighter(string Name, Point position);
	~Fighter() {};

	float getHealth(void) const {
		return health;
	}
	void removeHealth(float h) {
		if (health - h > 0) {
			health = health - h;
		}
		else health = 0;
	}
	void setStateMachine();
	void Handler();
	void Draw(SDL_Surface& gScreenSurface, Point enemy, Rect& camera);
	bool initialize(const string& path);//give json as argument
	Point GetPosition(void)const;
	int GetWin(void) const {
		return win;
	};
	void SetWin(void) {
		win++;
	}
};