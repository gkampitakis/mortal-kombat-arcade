#include <AnimationFilmHolder.h>
#include <assert.h>

AnimationFilmHolder* AnimationFilmHolder::holder = 0;

void AnimationFilmHolder::CleanUp(void) {
	for (Films::iterator i = films.begin(); i != films.end(); i++) delete(i->second);
	films.clear();
}

AnimationFilmHolder* AnimationFilmHolder::Get(void) {
	return holder;
};

void AnimationFilmHolder::Load(string &path, int framesNo, string id, SDL_Surface *screen) {
	SDL_Surface* bitmap = bitmaps.Load(path,screen->format);
	assert(!GetFilm(id));
	vector<Rect> test ;//HOLDER PLACEEEEEEEEE
	films[id] = new AnimationFilm(bitmap,test ,id);//FIXME:Here minimal boxes
}

const AnimationFilm* AnimationFilmHolder::GetFilm(string id) {
	Films::const_iterator i = films.find(id);
	return i != films.end() ? i->second : (const AnimationFilm*)0;
}
