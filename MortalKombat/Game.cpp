#include "Game.h"

Game::Game() {};

Game::~Game() {};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();
	//call the initialization of players

	subzero = new Fighter("subzero", {500,500});
	scorpion = new Fighter("scorpion",{900,500});
	if (!subzero->initialize("config/subzero_controller.json")) return false;
	if (!scorpion->initialize("config/scorpion_controller.json")) return false;
	
	return true;
};


void Game::DrawGame(SDL_Surface& gScreenSurface) {
	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitSurface(background, &camera, &gScreenSurface, &fullscreen);

	/*
		TESTING CODE
	*/
	//The camera might need moving or interaction with the players 
	subzero->Draw(gScreenSurface, "subzero.stance", 200,450);//test functions
	scorpion->Draw(gScreenSurface, "scorpion.stance", 240, 450);//test functions
};

void Game::CleanUp() {
	//Fix this 
};

void Game::HandleInput() {
	subzero->Handler();
	scorpion->Handler();
};