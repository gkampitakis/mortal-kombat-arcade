#include "Game.h"

Game::Game() {};

Game::~Game() {};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();

	//Init fonts
	Timerfont = TTF_OpenFont("media/font.ttf", 70);//The 28 here is the font size
	if (Timerfont == NULL)
	{
		cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
		return false;
	}

	//Fix here the color and size
	Namefont = TTF_OpenFont("media/font.ttf", 45);//The 28 here is the font size
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
	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitSurface(background, &camera, &gScreenSurface, &fullscreen);

	printTimer(gameTimer.ReverseTimer(90), { SCREEN_WIDTH / 2 - 35, 5 }, &gScreenSurface, { 198, 0, 10, 255 });

	//The camera might need moving or interaction with the playerres 
	subzero->Draw(gScreenSurface, "subzero.stance", 200, 450);//test functions
	scorpion->Draw(gScreenSurface, "scorpion.stance", 240, 450);//test functions
	//x,y, height/width changed the orientation in function
	RenderHpBarLeft(0.91f, gScreenSurface);//debuging normally subzero->getHealth();
	RenderHpBarRight(0.01f, gScreenSurface);//debuging normally scorpion->getHealth();
};

void Game::CleanUp() {
	//Fix this 
};

void Game::HandleInput() {
	if (!gameTimer.isStarted()) {//testing the timer wont start from here temporary place 
		gameTimer.start();
	}
	subzero->Handler();
	scorpion->Handler();
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