#include <Menu.h>
#include "AnimationFilm.h"
#include "MusicPlayer.h"

Menu::Menu() {
	time = 0;
}

bool Menu::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilmHolder::Get()->Load("media/menu.png", 1, "background", gScreenSurface);//the first pic is up
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("background");
	background = tmp->GetBitmap();
	//upload press start this needs testing about sprites and etc etc

	AnimationFilmHolder::Get()->Load("media/pressstart.png", 1, "presssstart", gScreenSurface);//the first pic is up
	tmp = AnimationFilmHolder::Get()->GetFilm("presssstart");

	SpriteHolder::Get()->Add(new Sprite({ (SCREEN_WIDTH / 2) - 70,SCREEN_HEIGHT - 100 }, tmp, SpriteTypes::MenuSprite));//zero is the type
	//also upload all the sprites for the main menu here 
	//the sound now needs the same thing  but ll see

	/* ADD SOUNDS HERE*/
	MusicPlayer::Get()->LoadEffect("media/test.wav","test");

	return true;
}

SDL_Surface* Menu::getBackground() {
	return background;
};

void Menu::DrawMenu(SDL_Surface& gScreenSurface) {
	//here also call a renderer for the sprites

	Rect fullscreen;
	fullscreen.w = SCREEN_WIDTH;
	fullscreen.h = SCREEN_HEIGHT;
	fullscreen.x = 0;
	fullscreen.y = 0;
	SDL_BlitScaled(background, NULL, &gScreenSurface, &fullscreen);
	unsigned int currTime = SDL_GetTicks();
	if (currTime > time + 500) {
		time = currTime;
		show = !show;
	}
	if (show) {
		Mix_Chunk* tmp= MusicPlayer::Get()->RetrieveEffect("test");//Here Working state
		MusicPlayer::Get()->PlayEffect(tmp,1);//Here Working state

		SpriteList menuSpriteList = SpriteHolder::Get()->GetSprites(SpriteTypes::MenuSprite);
		list<Sprite*>::iterator it;
		for (it = menuSpriteList.begin(); it != menuSpriteList.end(); ++it) {

			(*it)->DisplayUnique(gScreenSurface, 140, 50);
		};
	}
};