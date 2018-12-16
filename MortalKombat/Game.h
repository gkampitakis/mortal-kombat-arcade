#include "MyLibraries.h"
#include "TickTimerAnimator.h"
#include "TImer.h"
#include "Fighter.h"

class Game {
public:
	Game();
	~Game();
	bool initialize(SDL_Surface* gScreenSurface);
	void DrawGame(SDL_Surface& gScreenSurface);
	void CleanUp();
	void HandleInput(SDL_Event& event);
private:
	Fighter* subzero=NULL;
	Fighter* scorpion=NULL;
	TickTimerAnimator *timeAnimator;
	Timer* gameTimer;
};