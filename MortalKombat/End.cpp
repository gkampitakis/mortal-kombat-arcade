#include "End.h"
#include "AnimationFilmHolder.h"

End::End(Fighter* _winner, Fighter* _loser) {
	animatorW = new FrameRangeAnimator();
	animatorL = new FrameRangeAnimator();
	//Dummy coordinates for now
	winner = new Sprite({ 200,200 }, AnimationFilmHolder::Get()->GetFilm(_winner->GetName() + ".win"), SpriteTypes::FIGHTER);
	//Fix here losing stance
	loser = new Sprite({ 600,200 }, AnimationFilmHolder::Get()->GetFilm(_winner->GetName() + ".stance"), SpriteTypes::FIGHTER);
};

End::~End() {
	//free font 
	//and all animators and stuff
};

void End::Draw(SDL_Surface& surface) {
	Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_FillRect(&surface, &fullscreen, SDL_MapRGB(surface.format, 0, 0, 0));
};

bool End::HandleInput(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym==SDL_SCANCODE_ESCAPE) {
			return true;
			//Run all the cleanups here
			//MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("transition"), 0);
		}
	}
	return false;
};

void End::printMessage(const string& msg, Point position, SDL_Surface *surface, SDL_Color color, int fontsize) {

};