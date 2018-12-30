#include "MyLibraries.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "Fighter.h"

class End {
private:
	TTF_Font*		font = NULL;
	Sprite*			winner;
	Sprite*			loser;
	FrameRangeAnimator*		animatorW;
	FrameRangeAnimator*		animatorL;
	SDL_Surface*			background = NULL;
	//data

public:
	End(Fighter*, Fighter*);//Implement this
	~End();
	void printMessage(const string& msg, Point position, SDL_Surface *surface, SDL_Color color, int fontsize);
	void Draw(SDL_Surface& surface);
	bool HandleInput(SDL_Event& event);
};