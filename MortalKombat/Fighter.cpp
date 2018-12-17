#include "Fighter.h"
using Input = std::set<std::string>;

Fighter::Fighter(string Name) {
	name = Name;
};

bool Fighter::initialize() {
	input::key_combination test = { SDL_SCANCODE_W };
	//Here prolly a json file
	Fighter::inputController.AddAction(test, "UP");
	Fighter::stateTransitions.SetState("Ready");
	//Working for starters 
	//Now must see how the logical works and the state
	return true;
};

void Fighter::Draw(void) {
	//Not working
	//Fighter::stateTransitions.SetTransition(Fighter::stateTransitions.GetState(), Fighter::inputController.GetLogical(), []() {cout << "DAB"; });
}