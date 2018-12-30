#include "MyLibraries.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "Fighter.h"

class End {
private:
	TTF_Font*		font = NULL;
	Sprite*			winnerSprite;
	Sprite*			loserSprite;
	FrameRangeAnimator*		animatorW;
	FrameRangeAnimator*		animatorL;
	SDL_Surface*			background = NULL;
	Fighter* loser;
	Fighter* winner;

public:
	End(Fighter*, Fighter*, SDL_Surface& surface);//Implement this
	~End();
	void printMessage(const string& msg, Point position, SDL_Surface *surface, SDL_Color color, int fontsize);
	void Draw(SDL_Surface& surface);
	bool HandleInput(SDL_Event& event);
};