#include <MyLibraries.h>
#include <Sprite.h>
#include <AnimationFilmHolder.h>
#include <SpriteHolder.h>

class Menu {
public:
	Menu();
	~Menu() {};
	bool initialize(SDL_Surface* gScreenSurface);
	SDL_Surface* getBackground();
	void DrawMenu(SDL_Surface& gScreenSurface);
private:
	unsigned int time;
	bool show = true;
	SDL_Surface* background = NULL;
};