#include "MyLibraries.h"
#include "Sprite.h"
#include "AnimationFilmHolder.h"
#include "SpriteHolder.h"
#include "TickTimerAnimator.h"

class Intro {
public:
	Intro();
	~Intro() {};
	bool initialize(SDL_Surface* gScreenSurface);
	SDL_Surface* getBackground();
	void DrawIntro(SDL_Surface& gScreenSurface);
	int HandleInput(SDL_Event& event);
	void StartButton(SDL_Surface& gScreenSurface);

	void CleanUp(void);
private:
	SDL_Surface*			background = NULL;
	TickTimerAnimator*		timeAnimator;
};