#include "MyLibraries.h"
#include "Intro.h"
#include "Game.h"
#include "End.h"

using namespace std;

class Window {
public:
	Window(const int width, const int height);
	~Window();
	bool HandleInput(SDL_Event& event);
	bool open(string w_name);
	void close();

	bool initialize();
	void drawWindow();
	bool loadMedia();
	void drawDisclaimer(SDL_Surface& screen);

private:
	int				state = -1;
	int				width;
	int				height;
	SDL_Window*		window = NULL;
	SDL_Renderer*	gRenderer = NULL;
	SDL_Surface*	surface = NULL;
	TTF_Font*		font = NULL;
	Intro*			intro = NULL;
	Game*			game = NULL;
	End*			end = NULL;
	FrameRangeAnimator*		animator;
	Sprite*					sprite;
};