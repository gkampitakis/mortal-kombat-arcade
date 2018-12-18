#include "Game.h"

Game::Game() {};

Game::~Game() {};

bool Game::initialize(SDL_Surface* gScreenSurface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();
	//call the initialization of players

	subzero = new Fighter("subzero");
	scorpion = new Fighter("scorpion");
	if (!subzero->initialize("config/subzero_controller.json")) return false;
	if (!scorpion->initialize("config/scorpion_controller.json")) return false;
	//Here is called from window.cpp 80 line 
	return true;//this ll change
};


void Game::DrawGame(SDL_Surface& gScreenSurface) {
	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitSurface(background, &camera, &gScreenSurface, &fullscreen);
	//The camera might need moving or interaction with the players 
	Rect sub = {500,500,200,450};
	Rect sco = { 900,500,240,450 };
	subzero->Draw(gScreenSurface,"subzero.stance",sub);//test functions
	scorpion->Draw(gScreenSurface,"scorpion.stance",sco);
};

void Game::CleanUp() {

};

void Game::HandleInput() {
	//	inputController.Handle();
	subzero->getInputController().Handle();
	//nomizo edo oti to handle input mas prolabainei ka ipianei 2 events maybe needs fix 
	scorpion->getInputController().Handle();

	//Here might we are going to have each fighters handling input controller
};

//Write the setters and getters for the variables
//have a background 
