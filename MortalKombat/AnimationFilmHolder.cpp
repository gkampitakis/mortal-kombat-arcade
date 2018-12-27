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

void AnimationFilmHolder::Load(const string &path, const string &jsonpath, string id, SDL_Surface *screen, bool backgroundFlag) {
	SDL_Surface* bitmap = bitmaps.Load(path, screen->format, backgroundFlag);
	vector<Rect> boxes;
	assert(!GetFilm(id));

	try {
		json config;
		std::ifstream config_file(jsonpath, std::ifstream::binary);
		config_file >> config;
		/*
		* STRING MANIPULATION FOR THE DOT
		*/
		std::stringstream ss(id);
		const char delim = '.';
		std::vector<std::string> token;
		std::string s;
		while (std::getline(ss, s, delim)) {
			token.push_back(s);
		}

		string field = token[1];
		for (json::iterator i = config[field].begin(); i != config[field].end(); i++) {
			json tmpjson = *i;
			Rect tmp = { tmpjson["x"],tmpjson["y"],tmpjson["w"], tmpjson["h"] };
			boxes.push_back(tmp);
		}
		films[id] = new AnimationFilm(bitmap, boxes, id);
	}
	catch (const std::exception& e) {
		cerr << e.what();
	}
};

void AnimationFilmHolder::Load(const string &path, string id, SDL_Surface *screen, bool backgroundFlag) {
	SDL_Surface* bitmap = bitmaps.Load(path, screen->format, backgroundFlag);
	vector<Rect> boxes;
	assert(!GetFilm(id));
	Rect tmp = { 0,0,bitmap->w,bitmap->h };
	boxes.push_back(tmp);
	films[id] = new AnimationFilm(bitmap, boxes, id);
};

AnimationFilm* AnimationFilmHolder::GetFilm(string id) const {
	Films::const_iterator i = films.find(id);
	return i != films.end() ? i->second : (AnimationFilm*)0;
}
