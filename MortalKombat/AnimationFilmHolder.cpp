#include "AnimationFilmHolder.h"
#include <assert.h>

AnimationFilmHolder* AnimationFilmHolder::holder = new AnimationFilmHolder();

void AnimationFilmHolder::CleanUp(void) {
	for (Films::iterator i = films.begin(); i != films.end(); i++) delete(i->second);
	films.clear();
}

AnimationFilmHolder* AnimationFilmHolder::Get(void) {
	return holder;
};

void AnimationFilmHolder::Load(const string &path, int framesNo, string id, SDL_Surface *screen,bool backgroundFlag) {
	SDL_Surface* bitmap = bitmaps.Load(path,screen->format, backgroundFlag);
	assert(!GetFilm(id));
	vector<Rect> test ;//HOLDER PLACEEEEEEEEE
	films[id] = new AnimationFilm(bitmap,test ,id);//FIXME:Here minimal boxes
}

 AnimationFilm* AnimationFilmHolder::GetFilm(string id) const {
	Films::const_iterator i = films.find(id);
	return i != films.end() ? i->second : (AnimationFilm*)0;
}
