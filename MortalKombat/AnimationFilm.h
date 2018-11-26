#ifndef AnimationFilm_HEADER
#define AnimationFilm_HEADER


#include <MyLibraries.h>
#include <vector>

class AnimationFilm {
private:
	vector<Rect> boxes;
	SDL_Surface* bitmap;
	string id;
public:
	AnimationFilm(SDL_Surface* bitmap, const vector<Rect> boxes, string &id);
	~AnimationFilm() {};

	byte GetTotalFrames(void) const;
	SDL_Surface* GetBitmap(void) const;
	string GetId(void) const;
	Rect GetFrameBox(byte) const;
	void DisplayFrame(SDL_Surface &d, const Point &at, byte frameNo)const;
	void DisplayFrame(SDL_Surface &d, Rect rect) const;
};

#endif