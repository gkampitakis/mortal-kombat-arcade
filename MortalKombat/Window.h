#include <MyLibraries.h>
#include <Menu.h>

using namespace std;

class Window {
public:
	Window(const int width, const int height);
	~Window();
	bool HandleInput(SDL_Event& event);
	bool open(string w_name);
	void close();

	void initialize();

private:
	int width;
	int height;
	SDL_Window* window = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Surface* gScreenSurface = NULL;
	Menu* menu = NULL;
};