#include "Window.h"
#include "AnimatorHolder.h"

int main(int argc, char *args[]) {

	bool quit = false;
	Window new_window(SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Event event;

	if (!new_window.open("My window")) {
		cout << "Failed to initialize window";
	}
	else {
		if (!new_window.initialize()) {
			cerr << "Error with game initialazation\n";
			return 0;
		}
		while (!quit) {
			quit = new_window.HandleInput(event);
			new_window.drawWindow();
		}
	}
	new_window.close();
	return 0;
}