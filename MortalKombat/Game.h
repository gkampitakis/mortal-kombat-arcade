#include "MyLibraries.h"
#include "TickTimerAnimator.h"
#include "TImer.h"
#include "Fighter.h"
#include "AnimationFilmHolder.h"

class Game {
public:
	Game();
	~Game();
	bool initialize(SDL_Surface* gScreenSurface);
	void DrawGame(SDL_Surface& gScreenSurface);
	void CleanUp();
	void HandleInput(SDL_Event& event);
	static bool start;
	int GetRound(void)const;

private:
	Fighter*				subzero = NULL;
	Fighter*				scorpion = NULL;
	TickTimerAnimator*		timeAnimator;
	TickTimerAnimation*		timeAnimation;
	Timer					gameTimer;
	SDL_Surface*			background = NULL;
	SDL_Surface*			movingBckg = NULL;
	SDL_Rect				camera;
	TTF_Font*				Timerfont = NULL;
	TTF_Font*				Namefont = NULL;
	TTF_Font*				tmpFont = NULL;
	int						round;

	void matchWin(Fighter& fighter, SDL_Surface& gScreenSurface);
	void cameraAdjustment(void);
	void RenderHpBarRight(float percent, SDL_Surface& screen);
	void RenderHpBarLeft(float percent, SDL_Surface& screen);
	void printTimer(const std::string& msg, Point position, SDL_Surface *screen, SDL_Color color);
	void DelayAction(const std::function<void()>& f, delay_t d);
	void printMessage(const std::string& msg, Point position, SDL_Surface *gScreenSurface, SDL_Color color, int fontsize);
};