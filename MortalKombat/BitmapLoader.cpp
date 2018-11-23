#include <BitmapLoader.h>
#include <assert.h>
#include <MyLibraries.h>

SDL_Surface* BitmapLoader::GetBitmap(string path) const {
	Bitmaps::const_iterator i = bitmaps.find(path);
	return i != bitmaps.end() ? i->second : (SDL_Surface*) 0;
}

SDL_Surface * BitmapLoader::LoadBitmap(const string path, SDL_PixelFormat *format)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError();

	}
	else {
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface,format, NULL);
		if (optimizedSurface == NULL)
		{
			cout << "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError();
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}



SDL_Surface* BitmapLoader::Load(const string& path,SDL_PixelFormat *format) {
	SDL_Surface *b= GetBitmap(path);
	if (!b) {
		bitmaps[path] = (b = LoadBitmap(path,format));
		assert(b);
	}
	return b;
};

void BitmapLoader::CleanUp(void) {
	for (Bitmaps::iterator i = bitmaps.begin(); i != bitmaps.end(); ++i)
		SDL_FreeSurface(i->second);
	bitmaps.clear();
};