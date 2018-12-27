#ifndef AnimationFilmHolder_HEADER
#define AnimationFilmHolder_HEADER

#include "MyLibraries.h"
#include "AnimationFilm.h"
#include "BitmapLoader.h"

class AnimationFilmHolder {
private:
	AnimationFilmHolder(void) {};
	~AnimationFilmHolder() { CleanUp(); };

	typedef map<string, AnimationFilm*> Films;
	Films films;
	BitmapLoader bitmaps; //only for loading of film bitmaps
	static	AnimationFilmHolder* holder; //singleton
public:
	static AnimationFilmHolder* Get(void);
	//another GET exists at documentation
	void Load(const string &path, const string &jsonpath, string id, SDL_Surface *screen, bool backgroundFlag);
	void Load(const string &path, string id, SDL_Surface *screen, bool backgroundFlag);
	void CleanUp(void);
	AnimationFilm* GetFilm(const string) const;
};

#endif