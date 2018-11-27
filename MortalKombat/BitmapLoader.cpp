#include <BitmapLoader.h>
#include <assert.h>
#include <MyLibraries.h>

SDL_Surface* BitmapLoader::GetBitmap(const string path) const {
	Bitmaps::const_iterator i = bitmaps.find(path);
	return i != bitmaps.end() ? i->second : (SDL_Surface*)0;
}

SDL_Surface * BitmapLoader::LoadBitmap(const string path, SDL_PixelFormat *format, bool backgroundFlag)
{
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError();
	}
	else {
		if (!backgroundFlag) {
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));//Remove White Background
		}
		optimizedSurface = SDL_ConvertSurface(loadedSurface, format, NULL);
		if (optimizedSurface == NULL)
		{
			cout << "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError();
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}



SDL_Surface* BitmapLoader::Load(const string& path, SDL_PixelFormat *format, bool backgroundFlag) {
	SDL_Surface *b = GetBitmap(path);
	if (!b) {
		bitmaps[path] = (b = LoadBitmap(path, format, backgroundFlag));
		assert(b);
	}
	return b;
};

void BitmapLoader::CleanUp(void) {
	for (Bitmaps::iterator i = bitmaps.begin(); i != bitmaps.end(); ++i)
		SDL_FreeSurface(i->second);
	bitmaps.clear();
};