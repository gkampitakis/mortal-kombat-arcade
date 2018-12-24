#include "Game.h"
#include "AnimatorHolder.h"
#include "MusicPlayer.h"

bool Game::start = false;

Game::Game() {
	timeAnimator = new TickTimerAnimator(NULL);
	timeAnimation = new TickTimerAnimation(111);
	round = 1;
};

Game::~Game() {
	CleanUp();
};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();

	//Init fonts
	Timerfont = TTF_OpenFont("media/font.ttf", 70);
	if (Timerfont == NULL)
	{
		cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
		return false;
	}

	//Fix here the color and size
	Namefont = TTF_OpenFont("media/font.ttf", 45);
	if (Namefont == NULL)
	{
		cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
		return false;
	}
	//call the initialization of players

	subzero = new Fighter("subzero", { 200,500 });
	scorpion = new Fighter("scorpion", { 900,500 });
	if (!subzero->initialize("config/subzero_controller.json")) return false;
	if (!scorpion->initialize("config/scorpion_controller.json")) return false;

	return true;
};


void Game::DrawGame(SDL_Surface& gScreenSurface) {
	if (!gameTimer.isStarted() && start) {
		Game::start = false;
		cout << "end";
	}
	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitSurface(background, &camera, &gScreenSurface, &fullscreen);
	//For debugging purposes the timer is big 
	printTimer(gameTimer.ReverseTimer(1000), { SCREEN_WIDTH / 2 - 35, 5 }, &gScreenSurface, { 198, 0, 10, 255 });
	if (!start&&timeAnimator->GetState() == ANIMATOR_RUNNING) {
		printMessage("Round " + to_string(round), { SCREEN_WIDTH / 2 - 180,SCREEN_HEIGHT / 2 - 200 }, &gScreenSurface, { 255, 255, 0, 255 }, 150);
	}
	//The camera might need moving or interaction with the playerres 
	subzero->Draw(gScreenSurface,200,450);//test functions
	scorpion->Draw(gScreenSurface,240,450);//test functions
	//x,y, height/width changed the orientation in function
	RenderHpBarLeft(0.91f, gScreenSurface);//debuging normally subzero->getHealth();
	RenderHpBarRight(0.01f, gScreenSurface);//debuging normally scorpion->getHealth();
};

void Game::CleanUp() {
	//Fix this 
	//Clean fonts surfaces and all
	//call it at winow
};

void Game::HandleInput(SDL_Event& event) {
	if (!Game::start) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_SPACE) {
				DelayAction([&]() {
					AnimatorHolder::Remove(timeAnimator);
					Game::start = true;
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("fight"), 0);
					gameTimer.start();
				}, 1000);//A bit more time here but for debug purposes leave it fast
			}
		}
	}
	else {
		subzero->Handler();
		scorpion->Handler();
	}
};


void Game::printTimer(const std::string& msg, Point position, SDL_Surface *gScreenSurface, SDL_Color color) {
	SDL_Rect dest = { position.x,position.y,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(Timerfont, msg.c_str(), color);

	if (stext) {
		SDL_BlitSurface(stext, NULL, gScreenSurface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}
};

void Game::RenderHpBarRight(float healt, SDL_Surface& gScreenSurface) {
	Rect bar = { 679, 65, 45, 580 };
	healt = healt > 1.f ? 1.f : healt < 0.f ? 0.f : healt;

	SDL_Surface* tmp = SDL_CreateRGBSurface(0, bar.h, bar.w, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 198, 0, 10));
	SDL_BlitSurface(tmp, NULL, &gScreenSurface, &bar);
	SDL_FreeSurface(tmp);

	int pw = (int)((float)bar.w * healt);

	tmp = SDL_CreateRGBSurface(0, pw, bar.h, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 0, 128, 0));
	Rect tempRect = { bar.x + (bar.w - pw),bar.y,bar.w,bar.h };
	SDL_BlitSurface(tmp, NULL, &gScreenSurface, &tempRect);
	SDL_FreeSurface(tmp);

	//NAME 
	SDL_Rect dest = { 1055,65,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(Namefont, "SCORPION", { 255, 255, 0, 255 });
	if (stext) {
		SDL_BlitSurface(stext, NULL, &gScreenSurface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}

};

void Game::RenderHpBarLeft(float healt, SDL_Surface& gScreenSurface) {
	Rect bar = { 24, 65, 45, 580 };
	healt = healt > 1.f ? 1.f : healt < 0.f ? 0.f : healt;

	SDL_Surface* tmp = SDL_CreateRGBSurface(0, bar.h, bar.w, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 198, 0, 10));
	SDL_BlitSurface(tmp, NULL, &gScreenSurface, &bar);
	SDL_FreeSurface(tmp);

	int pw = (int)((float)bar.w * healt);

	tmp = SDL_CreateRGBSurface(0, pw, bar.h, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 0, 128, 0));
	SDL_BlitSurface(tmp, NULL, &gScreenSurface, &bar);
	SDL_FreeSurface(tmp);

	//Name
	SDL_Rect dest = { 50,65,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(Namefont, "SUBZERO", { 255, 255, 0, 255 });
	if (stext) {
		SDL_BlitSurface(stext, NULL, &gScreenSurface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}

};

void Game::DelayAction(const std::function<void()>& f, delay_t d) {
	if (timeAnimator&&timeAnimator->GetState() != ANIMATOR_RUNNING) {
		timeAnimation->setOnTick([] {
			//Nothing to do here
		}).SetDelay(d).SetReps(1);
		timeAnimator = new TickTimerAnimator(timeAnimation);
		timeAnimator->SetOnFinish(f);
		timeAnimator->Start(SDL_GetTicks());
		AnimatorHolder::MarkAsRunning(timeAnimator);
	}
};


void Game::printMessage(const std::string& msg, Point position, SDL_Surface *gScreenSurface, SDL_Color color, int fontsize) {

	SDL_Rect dest = { position.x,position.y,0,0 };
	tmpFont = TTF_OpenFont("media/font.ttf", fontsize);
	if (Namefont == NULL)
	{
		throw
			std::string("Failed to load lazy font! SDL_ttf Error");
	}
	SDL_Surface *stext = TTF_RenderText_Blended(tmpFont, msg.c_str(), color);

	if (stext) {
		SDL_BlitSurface(stext, NULL, gScreenSurface, &dest);
		SDL_FreeSurface(stext);
		TTF_CloseFont(tmpFont);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
		TTF_CloseFont(tmpFont);
	}

};




