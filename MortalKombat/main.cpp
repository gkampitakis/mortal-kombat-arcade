#include <Window.h>
//#include <MyLibraries.h>

int main(int argc, char *args[]) {
	bool quit = false;

	Window new_window(SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_Event event;

	if (!new_window.open("My window")) {
		cout << "Failed to initialize window";
	}
	else {
		new_window.initialize();
		while (!quit) {
			quit = new_window.HandleInput(event);
		}
		//new_window.draw(); Here should be implemented
	}
	new_window.close();
	return 0;
}