#include <AnimationFilm.h>
#include <assert.h>

AnimationFilm::AnimationFilm(SDL_Surface* bitmap, const vector<Rect> boxes, string &id) {
	this->id = id;
	this->bitmap = bitmap;
	this->boxes = boxes;
};//check if this works


byte AnimationFilm::GetTotalFrames(void) const {
	return (byte) boxes.size();
};

SDL_Surface* AnimationFilm::GetBitmap(void) const {
	return bitmap;
};

string AnimationFilm::GetId(void) const {
	return id;
};

Rect AnimationFilm::GetFrameBox(byte frameNo) const {
	assert(boxes.size() > frameNo); 
	return boxes[frameNo];
};

void AnimationFilm::DisplayFrame(SDL_Surface &d, const Point &at, byte frameNo) const {
	//maybe well need some othe functions like these too
	Rect rect = { at.x,at.y,0,0};
	SDL_BlitScaled(bitmap, &GetFrameBox(frameNo), &d, &rect);
};
