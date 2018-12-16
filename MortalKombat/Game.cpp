#include "Game.h"

Game::Game() {};

Game::~Game() {};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	//Here is called from window.cpp 80 line 
	return true;//this ll change
};


void Game::DrawGame(SDL_Surface& gScreenSurface) {
	SDL_FillRect(&gScreenSurface, NULL, 0x000000);
};

void Game::CleanUp() {

};

void Game::HandleInput(SDL_Event& event) {
	//Here might we are going to have each fighters handling input controller
};

//Write the setters and getters for the variables
//have a background 