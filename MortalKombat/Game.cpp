#include "Game.h"

Game::Game() {};

Game::~Game() {};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();
	//Here is called from window.cpp 80 line 
	return true;//this ll change
};


void Game::DrawGame(SDL_Surface& gScreenSurface) {
	Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Rect camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitSurface(background, &camera, &gScreenSurface, &fullscreen);
	//The camera might need moving or interaction with the players 
};

void Game::CleanUp() {

};

void Game::HandleInput(SDL_Event& event) {
	//Here might we are going to have each fighters handling input controller
};

//Write the setters and getters for the variables
//have a background 
