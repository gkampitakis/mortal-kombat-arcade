#ifndef Fighter_HEADER
#define Fighter_HEADER

#include "MyLibraries.h"
#include "Sprite.h"
#include "TickTimerAnimator.h"
#include "TickTimerAnimation.h"
#include "InputContoller.h"
#include "StateTransitions.h"
#include "FrameRangeAnimator.h"

#define	FIGHTER_ACTION_DELAY_MSECS	150

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
	struct fightstasts {
		int received_kicks = 0;
		int received_punches = 0;
		int blocked = 0;
		int received_signif_strikes = 0;
	} fightstasts;
	// TODO: 
	Fighter(string Name, Point position);
	~Fighter() {};

	void Handler();
	void Draw(SDL_Surface& surface, Point enemy, Rect& camera);
	bool initialize(const string& path);//give json as argument

	float getHealth(void) const;
	void ResetHealth(void);
	void removeHealth(float h);

	int GetWin(void) const;;
	void SetWin(void);

	string GetName(void);
	void setStateMachine();
	Point GetPosition(void)const;
	
	void SetState(string state);
	string GetState(void) const;

	bool proximityDetector(Sprite* enemy);
	Sprite* GetSprite(void)const;
	string GetAction(void)const;
	void ResetPosition(Point x);
	void InflictionAnimation(string Animation,int speed,string hit);
};

#endif