#include <Window.h>

Window::Window(const int width, const int height) {
	this->width = width;
	this->height = height;
};

Window::~Window() {};

bool Window::HandleInput(SDL_Event& event) {
	bool quit = false;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			quit = true;
		}
		else {
			if (state == state::MENU) {//This will be changed
				menu->HandleInput(event);
			}
		}
	}
	return quit;
};

bool Window::open(string w_name) {

	//here do a controller check maybe
	//For starting these
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	return true;
};


void Window::initialize() {
	//Here do call the picture/sound/sprite and etc initialazation
//	if (!LoadMedia()) {
//		cout << "Something went really bad";
	//}
	menu = new Menu();
	menu->initialize(gScreenSurface);
	state = state::MENU;
	loadMedia();//This needs fix
};

void Window::close() {
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	window = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
};


bool Window::loadMedia() {//Needs fix
	//Here load the font 
	return true;
}


void Window::drawWindow() {
	if (state == state::MENU) {
		menu->DrawMenu(*gScreenSurface);
	}
	//This also should not be here 
	SDL_UpdateWindowSurface(window);
};