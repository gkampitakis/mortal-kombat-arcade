#include "Window.h"
#include "AnimatorHolder.h"
#include "MusicPlayer.h"

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
			//if here for the sound to handle "globaly"
			if (state == INTRO) {
				int tmpstate = intro->HandleInput(event);
				state = tmpstate == -1 ? state : tmpstate;
			}
			else if (state == INGAME) {
				game->HandleInput(event);
			}
		}//Handle Input For Menu
	}
	return quit;
};

bool Window::open(string w_name) {

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

	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return  false;
	}
	return true;
};


bool Window::initialize() {
	//Here do call the picture/sound/sprite and etc initialazation
	if (!loadMedia()) {
		cout << "Something went really bad\n";
		return false;
	}
	intro = new Intro();
	if (!intro->initialize(gScreenSurface)) return false;
	//Initialize Menu
	state = INTRO;

	game = new Game();
	if (!game->initialize(gScreenSurface)) return false;
	return true;
};

void Window::close() {

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);

	window = NULL;
	gRenderer = NULL;
	gScreenSurface = NULL;

	//Call the Holder's CleanUp Functions
	SpriteHolder::Get()->CleanUp();
	AnimatorHolder::CleanUp();
	AnimationFilmHolder::Get()->CleanUp();
	MusicPlayer::Get()->CleanUp();

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
};


bool Window::loadMedia() {
	//Here load the font 
	font = TTF_OpenFont("media/font.ttf", 70);
	if (font == NULL)
	{
		cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
		return false;
	}
	try {
		/*
		*		FILMS LOADING HERE
		*/
		AnimationFilmHolder::Get()->Load("media/win.png", "win", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/disclaimer.png", "disclaimer", gScreenSurface, true);
		AnimationFilmHolder::Get()->Load("media/stage.png", "stage", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/bckg.png", "bckg", gScreenSurface, true);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.stance", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.stance", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.move", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.move", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.up", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.up", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.down", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.down", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.duck", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.duck", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.block", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.block", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.punch1", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.punch1", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.punch2", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.punch2", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.kick1", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.kick1", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.kick2", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.kick2", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.stillblock", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.stillblock", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.flipup", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.lowkick", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.lowpunch", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.highpunch", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.highkick1", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.highkick2", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.lowpunch", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.lowkick", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.highpunch", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.highkick1", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.highkick2", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.flipdown", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.downblock", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.downblock", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.flipup", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.flipdown", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/subzero.png", "config/subzero_boxes.json", "subzero.win", gScreenSurface, false);
		AnimationFilmHolder::Get()->Load("media/scorpion.png", "config/scorpion_boxes.json", "scorpion.win", gScreenSurface, false);
		
		/*
		*		SOUND LOADING HERE
		*/
		MusicPlayer::Get()->LoadEffect("media/fight.wav", "fight");
		MusicPlayer::Get()->LoadMusic("media/intro.wav", "intro");
		MusicPlayer::Get()->PlayMusic(MusicPlayer::Get()->RetrieveMusic("intro"));
		MusicPlayer::Get()->LoadEffect("media/transition.wav", "transition");
		MusicPlayer::Get()->LoadEffect("media/subzerowins.wav", "subzero.wins");
		MusicPlayer::Get()->LoadEffect("media/scorpionwins.wav", "scorpion.wins");
		MusicPlayer::Get()->LoadEffect("media/kick1.wav", "kickWave");
		MusicPlayer::Get()->LoadEffect("media/punch1.wav", "punchWave");
		MusicPlayer::Get()->LoadEffect("media/block.wav", "block");
		MusicPlayer::Get()->LoadEffect("media/singlehit.wav", "singlehit");

	}
	catch (const std::exception& e) {
		cerr << e.what();
		return false;
	}
	return true;
}


void drawDisclaimer(SDL_Surface& gScreenSurface) {
	SDL_Surface* background = NULL;
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("disclaimer");
	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	tmp->DisplayFrame(gScreenSurface, fullscreen);
};


void Window::drawWindow() {

	if (state == INTRO) {
		intro->DrawIntro(*gScreenSurface);
	}
	else if (state == DISCLAIMER) {
		static bool initiate;
		drawDisclaimer(*gScreenSurface);
		if (!initiate) {
			initiate = true;
			TickTimerAnimation* tmp2 = new TickTimerAnimation(1);
			tmp2->setOnTick([] {
				//Nothing to do here
			}).SetDelay(1000).SetReps(1);
			//10000 set delay for not waiting left 100
			TickTimerAnimator* timeAnimator = new TickTimerAnimator(tmp2);
			timeAnimator->SetOnFinish([&]() {
				AnimatorHolder::Remove(timeAnimator);
				state = INGAME;
			});
			timeAnimator->Start(SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(timeAnimator);
		}
	}
	else if (state == INGAME) {
		game->DrawGame(*gScreenSurface);
	}
	AnimatorHolder::Progress(SDL_GetTicks());
	SDL_UpdateWindowSurface(window);
};