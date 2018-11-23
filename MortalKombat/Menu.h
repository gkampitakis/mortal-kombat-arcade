#include <MyLibraries.h>

class Menu {
public:
	Menu();
	~Menu();
	bool initialize(SDL_Surface* gScreenSurface);
	SDL_Surface* getBackground();

private:
	bool visible = true;
	SDL_Surface* background = NULL;
};