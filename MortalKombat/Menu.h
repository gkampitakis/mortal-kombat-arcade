#include <MyLibraries.h>
#include <Sprite.h>
#include <AnimationFilmHolder.h>

class Menu {
public:
	Menu();
	~Menu();
	bool initialize(SDL_Surface* gScreenSurface);
	SDL_Surface* getBackground();
	void DrawMenu(SDL_Surface* gScreenSurface);
private:
	bool visible = true;
	SDL_Surface* background = NULL;
};