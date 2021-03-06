#include "Intro.h"
#include "AnimationFilm.h"
#include "MusicPlayer.h"
#include "TickTimerAnimation.h"
#include "AnimatorHolder.h"

Intro::Intro() {}

bool Intro::initialize(SDL_Surface* surface) {
	AnimationFilmHolder::Get()->Load("media/menu.png", "background", surface, true);
	AnimationFilmHolder::Get()->Load("media/pressstart.png", "presssstart", surface, false);
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("background");
	background = tmp->GetBitmap();

	tmp = AnimationFilmHolder::Get()->GetFilm("presssstart");
	SpriteHolder::Get()->Add(new Sprite({ (SCREEN_WIDTH / 2) - 70,SCREEN_HEIGHT - 120 }, tmp, SpriteTypes::INTRO_SPRITE));

	StartButton(*surface);

	return true;
}

SDL_Surface* Intro::getBackground() {
	return background;
};

void Intro::DrawIntro(SDL_Surface& surface) {

	Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitScaled(background, NULL, &surface, &fullscreen);

	SpriteList menuSpriteList = SpriteHolder::Get()->GetSprites(SpriteTypes::INTRO_SPRITE);
	list<Sprite*>::iterator it;
	for (it = menuSpriteList.begin(); it != menuSpriteList.end(); ++it) {
		(*it)->DisplayUnique(surface, 140, 50);
	};
};

int Intro::HandleInput(SDL_Event& event) {//Handle input for menu
	int state = -1;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym) {
			state = DISCLAIMER;
			CleanUp();
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("transition"), 0);
		}
	}
	return state;
}

void Intro::StartButton(SDL_Surface& surface) {

	TickTimerAnimation* tmp2 = new TickTimerAnimation(1);
	tmp2->setOnTick([&](void) {
		SpriteList menuSpriteList = SpriteHolder::Get()->GetSprites(SpriteTypes::INTRO_SPRITE);
		list<Sprite*>::iterator it;

		for (it = menuSpriteList.begin(); it != menuSpriteList.end(); ++it) {//This here might change or just have one item here future evaluation
			(*it)->SetVisibility(!(*it)->IsVisible());//Here can we find the specific sprite and not do an iteration
		};
	}).SetDelay(500).SetReps(0);
	timeAnimator = new TickTimerAnimator(tmp2);
	timeAnimator->SetOnFinish([&]() {
		AnimatorHolder::Remove(timeAnimator);
	});
	timeAnimator->Start(SDL_GetTicks());
	AnimatorHolder::MarkAsRunning(timeAnimator);
}


void Intro::CleanUp(void) {
	//Cleaning the animators called here 
	AnimatorHolder::CleanUp();
	MusicPlayer::Get()->StopMusic();
}