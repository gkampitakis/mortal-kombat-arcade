#include <MyLibraries.h>

class BitmapLoader {
private:
	typedef map<string, SDL_Surface*> Bitmaps;
	Bitmaps bitmaps;
	SDL_Surface* GetBitmap(const string path) const;
	SDL_Surface* LoadBitmap(const string path, SDL_PixelFormat *format);
public:
	BitmapLoader(void) {};
	~BitmapLoader() { CleanUp(); };

	SDL_Surface* Load(const string& path, SDL_PixelFormat *format);
	void CleanUp(void);
};