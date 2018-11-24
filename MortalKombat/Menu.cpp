#include <Menu.h>
#include "AnimationFilm.h"

Menu::Menu() {


}

bool Menu::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilmHolder::Get()->Load("media/menu.png",1,"background",gScreenSurface);//the first pic is up
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("background");
	background = tmp->GetBitmap();
	//also upload all the sprites for the main menu here 
	//the sound now needs the same thing  but ll see
	return true;
}

SDL_Surface* Menu::getBackground() {
	return background;
};

void Menu::DrawMenu(SDL_Surface* gScreenSurface) {
	//here also call a renderer for the sprites
	Rect fullscreen;//here write it clear
	fullscreen.w = SCREEN_WIDTH;
	fullscreen.h = SCREEN_HEIGHT;
	fullscreen.x = 0;
	fullscreen.y = 0;
	SDL_BlitScaled(background,NULL, gScreenSurface, &fullscreen);
}