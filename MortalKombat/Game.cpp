#include "Game.h"

Game::Game() {};

Game::~Game() {};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();
	//call the initialization of players

	subzero = new Fighter("subzero", { 500,500 });
	scorpion = new Fighter("scorpion", { 900,500 });
	if (!subzero->initialize("config/subzero_controller.json")) return false;
	if (!scorpion->initialize("config/scorpion_controller.json")) return false;

	return true;
};


void Game::DrawGame(SDL_Surface& gScreenSurface, TTF_Font& font) {
	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitSurface(background, &camera, &gScreenSurface, &fullscreen);

	printMessageAt(gameTimer.ReverseTimer(90), { SCREEN_WIDTH/2-35, 20 }, &font, &gScreenSurface, { 198, 0, 10, 255 });

	/*
		TESTING CODE
	*/
	//The camera might need moving or interaction with the playerres 
	subzero->Draw(gScreenSurface, "subzero.stance", 200, 450);//test functions
	scorpion->Draw(gScreenSurface, "scorpion.stance", 240, 450);//test functions
};

void Game::CleanUp() {
	//Fix this 
};

void Game::HandleInput() {
	if (!gameTimer.isStarted()) {//testing the timer wont start from here 
		gameTimer.start();
	}
	subzero->Handler();
	scorpion->Handler();
};

void Game::printMessageAt(const std::string& msg, Point position, TTF_Font *myFont, SDL_Surface *gScreenSurface, SDL_Color color) {
	SDL_Rect dest = { position.x,position.y,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(myFont, msg.c_str(), color);

	if (stext) {
		SDL_BlitSurface(stext, NULL, gScreenSurface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}
}