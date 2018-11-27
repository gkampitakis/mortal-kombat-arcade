#include <MyLibraries.h>
#include <Menu.h>

using namespace std;
enum state { MENU, INGAME, FINISH, SCOREBOARD, OPTIONS };//This states might change or add new states

class Window {
public:
	Window(const int width, const int height);
	~Window();
	bool HandleInput(SDL_Event& event);
	bool open(string w_name);
	void close();

	void initialize();
	void drawWindow();
	bool loadMedia();

private:
	int state=-1;
	int width;
	int height;
	SDL_Window* window = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Surface* gScreenSurface = NULL;
	//FONT add here 
	Menu* menu = NULL;
};