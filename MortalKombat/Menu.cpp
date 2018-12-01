#include "Menu.h"
#include "AnimationFilm.h"
#include "MusicPlayer.h"
#include "TickTimerAnimation.h"
#include "AnimatorHolder.h"

Menu::Menu() {}

bool Menu::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilmHolder::Get()->Load("media/menu.png", 1, "background", gScreenSurface,true);//the first pic is up
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("background");
	background = tmp->GetBitmap();

	AnimationFilmHolder::Get()->Load("media/pressstart.png", 1, "presssstart", gScreenSurface,false);//the first pic is up
	tmp = AnimationFilmHolder::Get()->GetFilm("presssstart");

	SpriteHolder::Get()->Add(new Sprite({ (SCREEN_WIDTH / 2) - 70,SCREEN_HEIGHT - 100 }, tmp, SpriteTypes::MenuSprite));
	//also upload all the sprites for the main menu here 
	//the sound now needs the same thing  but ll see

	/* ADD SOUNDS HERE*/
	//MusicPlayer::Get()->LoadEffect("media/test2.wav","test"); //Testing Load It works but too loud
	StartButton(*gScreenSurface);
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
	
	SpriteList menuSpriteList = SpriteHolder::Get()->GetSprites(SpriteTypes::MenuSprite);
	list<Sprite*>::iterator it;
	for (it = menuSpriteList.begin(); it != menuSpriteList.end(); ++it) {
		(*it)->DisplayUnique(gScreenSurface, 140, 50);
	};
};

void Menu::HandleInput(SDL_Event& event) {//A basic handle input for menu
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym)
		{
		case SDLK_0:
			cout << "0";
			break;
		case SDLK_1:
			cout << "1";
			break;
		case SDLK_2:
			cout << "2";
			break;
		default:
			cout << "Unknow event";
			break;
		}
	}
	//cout <<&event <<"caught a event";
}

void Menu::StartButton(SDL_Surface& gScreenSurface) {
	
	TickTimerAnimation* tmp2 = new TickTimerAnimation(1);
	tmp2->setOnTick([&](void) {
		SpriteList menuSpriteList = SpriteHolder::Get()->GetSprites(SpriteTypes::MenuSprite);
		list<Sprite*>::iterator it;

		for (it = menuSpriteList.begin(); it != menuSpriteList.end(); ++it) {//This here might change or just have one item here future evaluation
			(*it)->SetVisibility(!(*it)->IsVisible());//Here can we find the specific sprite and not do an iteration
		};
	}).SetDelay(500).SetReps(0);

	timeAnimator = new TickTimerAnimator(tmp2);
	timeAnimator->Start(SDL_GetTicks());
	AnimatorHolder::MarkAsRunning(timeAnimator);
}


