#include "InputContoller.h"

using All = std::list<input::InputController*>;

All input::InputController::all;


bool input::test_key(const int keyCode) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[keyCode]) {
		return true;
	}
	else return  false;
}