#include <MyLibraries.h>
#include <AnimationFilm.h>
#include <BitmapLoader.h>

class AnimationFilmHolder {
private:
	AnimationFilmHolder(void){};
	~AnimationFilmHolder() { CleanUp(); };

	typedef map<string, AnimationFilm*> Films;
	Films films;
	BitmapLoader bitmaps; //only for loading of film bitmaps
	static	AnimationFilmHolder* holder; //singleton
public:
	static AnimationFilmHolder* Get(void);
	//another GET exists at documentation
	void Load(const string &path, int framesNo, string id, SDL_Surface *screen,bool backgroundFlag);
	void CleanUp(void);
	AnimationFilm* GetFilm(const string) const;
};