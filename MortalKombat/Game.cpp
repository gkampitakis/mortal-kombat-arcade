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
	Rect sub = { 500,500,200,450 };
	Rect sco = { 900,500,240,450 };
	subzero->Draw(gScreenSurface, "subzero.stance", sub);//test functions
	scorpion->Draw(gScreenSurface, "scorpion.stance", sco);//test functions
};

void Game::CleanUp() {

};

void Game::HandleInput() {
	subzero->Handler();
	scorpion->Handler();
};
/*

void Game::InitSubzeroStates(logic::StateTransitions& states) {
	using Input = logic::StateTransitions::Input;
	states.
		SetTransition("READY", Input{ "HPUNCH" }, [](void) {

	})
		.SetTransition("READY", Input{ "BCK" }, [](void) {

	})
		.SetTransition("READY", Input{ "FWD" }, [](void) {

	})
		.SetTransition("READY", Input{ "DOWN" }, [](void) {

	})
		.SetTransition("READY", Input{ "UP" }, [](void) {
		cout << "Jumping\n";
	})
		.SetTransition("READY", Input{ "DOWN","FWD","LPUNCH" }, [](void) {

	})
		.SetTransition("READY", Input{}, [](void) {
		cout << "Waiting\n";
	})
		.SetTransition("DOWN", Input{ "DOWN","BLOCK" }, [](void) {

	})
		.SetTransition("DOWN", Input{ "DOWN","HPUNCH" }, [](void) {

	})
		.SetTransition("DOWN", Input{}, [](void) {
		cout << "Getting Up\n";
	});
	//dont forget the defaults
};


//maybe this is not needed two times
void Game::InitScorpionStates(logic::StateTransitions& states) {
	using Input = logic::StateTransitions::Input;
	states.
		SetTransition("READY", Input{ "HPUNCH" }, [](void) {

	})
		.SetTransition("READY", Input{ "BCK" }, [](void) {

	})
		.SetTransition("READY", Input{ "FWD" }, [](void) {

	})
		.SetTransition("READY", Input{ "DOWN" }, [](void) {

	})
		.SetTransition("READY", Input{ "UP" }, [](void) {
		cout << "Jumping\n";
	})
		.SetTransition("READY", Input{ "DOWN","FWD","LPUNCH" }, [](void) {

	})
		.SetTransition("READY", Input{}, [](void) {
		cout << "Waiting\n";
	})
		.SetTransition("DOWN", Input{ "DOWN","BLOCK" }, [](void) {

	})
		.SetTransition("DOWN", Input{ "DOWN","HPUNCH" }, [](void) {

	})
		.SetTransition("DOWN", Input{}, [](void) {
		cout << "Getting Up\n";
	});
};
*/

