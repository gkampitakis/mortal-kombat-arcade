#include "End.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"

End::End(Fighter* _winner, Fighter* _loser, SDL_Surface& surface) {
	animatorW = new FrameRangeAnimator();
	animatorL = new FrameRangeAnimator();
	winner = _winner;
	loser = _loser;
	//Dummy coordinates for now
	winnerSprite = new Sprite({ SCREEN_WIDTH / 4-145,30 }, AnimationFilmHolder::Get()->GetFilm(_winner->GetName() + ".win"), SpriteTypes::FIGHTER);
	//Fix here losing stance
	loserSprite = new Sprite({ 3 * SCREEN_WIDTH / 4-55,30 }, AnimationFilmHolder::Get()->GetFilm(_loser->GetName() + ".dizzy"), SpriteTypes::FIGHTER);

	animatorW->Start(winnerSprite,
		new FrameRangeAnimation(0, winnerSprite->getFilm()->GetTotalFrames(), 0, 0, 300, true, 150),
		SDL_GetTicks());

	animatorL->Start(loserSprite,
		new FrameRangeAnimation(0, loserSprite->getFilm()->GetTotalFrames(), 0, 0, 250, true, 150),
		SDL_GetTicks());
	AnimatorHolder::MarkAsRunning(animatorL);
	AnimatorHolder::MarkAsRunning(animatorW);
};

End::~End() {
	//free font 
	//and all animators and stuff
};

void End::Draw(SDL_Surface& surface) {
	Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_FillRect(&surface, &fullscreen, SDL_MapRGB(surface.format, 0, 0, 0));

	winnerSprite->Display(surface, 300, 500);
	loserSprite->Display(surface, 300, 500);
	/* LINE*/
	SDL_Surface* tmp = SDL_CreateRGBSurface(0, 1150, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 255, 255, 0));
	SDL_BlitSurface(tmp, NULL, &surface, new Rect{ SCREEN_WIDTH / 2 - 575,570,0,0 });
	SDL_FreeSurface(tmp);

	printMessage("Presss ESC to Exit", { SCREEN_WIDTH / 2,850 }, &surface, { 192, 192,192, 255 }, 30);

	/*STATS*/

	printMessage("Punches Landed: " + to_string(loser->fightstasts.received_punches), { SCREEN_WIDTH / 4,600 }, &surface, { 255, 255,0, 255 }, 30);
	printMessage("Kicks Landed: " + to_string(loser->fightstasts.received_kicks), { SCREEN_WIDTH / 4,650 }, &surface, { 255, 255,0, 255 }, 30);
	printMessage("Significant Strikes Landed: " + to_string(loser->fightstasts.received_signif_strikes), { SCREEN_WIDTH / 4,700 }, &surface, { 255, 255,0, 255 }, 30);
	printMessage("Hits Blocked: " + to_string(winner->fightstasts.blocked), { SCREEN_WIDTH / 4,750 }, &surface, { 255, 255,0, 255 }, 30);

	printMessage("Punches Landed: " + to_string(winner->fightstasts.received_punches), { 3 * SCREEN_WIDTH / 4,600 }, &surface, { 255, 255,0, 255 }, 30);
	printMessage("Kicks Landed: " + to_string(winner->fightstasts.received_kicks), { 3 * SCREEN_WIDTH / 4,650 }, &surface, { 255, 255,0, 255 }, 30);
	printMessage("Significant Strikes Landed: " + to_string(winner->fightstasts.received_signif_strikes), { 3 * SCREEN_WIDTH / 4,700 }, &surface, { 255, 255,0, 255 }, 30);
	printMessage("Hits Blocked: " + to_string(loser->fightstasts.blocked), { 3 * SCREEN_WIDTH / 4,750 }, &surface, { 255, 255,0, 255 }, 30);

};

bool End::HandleInput(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			return true;
			//Run all the cleanups here
			//MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("transition"), 0);
		}
	}
	return false;
};

void End::printMessage(const string& msg, Point position, SDL_Surface *surface, SDL_Color color, int fontsize) {

	font = TTF_OpenFont("media/font.ttf", fontsize);
	if (font == NULL)
	{
		throw
			std::string("Failed to load lazy font! SDL_ttf Error");
	}
	SDL_Surface *stext = TTF_RenderText_Blended(font, msg.c_str(), color);
	SDL_Rect dest = { (position.x - stext->w / 2),position.y,0,0 };

	if (stext) {
		SDL_BlitSurface(stext, NULL, surface, &dest);
		SDL_FreeSurface(stext);
		TTF_CloseFont(font);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
		TTF_CloseFont(font);
	}
};