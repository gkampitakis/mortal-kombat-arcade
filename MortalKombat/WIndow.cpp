#include <Window.h>

Window::Window(const int width, const int height) {
	this->width = width;
	this->height = height;
}

Window::~Window() {}

bool Window::HandleInput(SDL_Event& event) {
	bool quit = false;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			quit = true;
		}
	}
	return quit;
}

bool Window::open(string w_name) {

	//here do a controller check maybe
	//For starting these
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cout << "Warning: Linear texture filtering not enabled!";
	}
	window = SDL_CreateWindow(w_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "Window could not be created! SDL Error: %s\n", SDL_GetError();
		return false;
	}
	gScreenSurface = SDL_GetWindowSurface(window);
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL) {
		cout << "Renderer could not be created! SDL Error: %s\n", SDL_GetError();
		return false;
	}
	return true;
}


void Window::initialize() {
	//Here do call the picture/sound/sprite and etc initialazation
	menu = new Menu();

	menu->initialize(gScreenSurface);
	SDL_Rect fullscreen;
	fullscreen.w = SCREEN_WIDTH;
	fullscreen.h = SCREEN_HEIGHT;
	fullscreen.x = 0;
	fullscreen.y = 0;

	SDL_BlitScaled(menu->getBackground(), NULL, gScreenSurface, &fullscreen);

	//Update the surface
	SDL_UpdateWindowSurface(window);

}

void Window::close() {
	//For starting

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	window = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}