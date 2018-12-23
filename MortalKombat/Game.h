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

private:
	Fighter* subzero = NULL;
	Fighter* scorpion = NULL;
	TickTimerAnimator *timeAnimator;
	Timer gameTimer;
	SDL_Surface* background = NULL;

	TTF_Font *Timerfont = NULL;
	TTF_Font *Namefont = NULL;

	void RenderHpBarRight(float percent, SDL_Surface& screen);
	void RenderHpBarLeft(float percent, SDL_Surface& screen);
	void printTimer(const std::string& msg, Point position, SDL_Surface *screen, SDL_Color color);
};