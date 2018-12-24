#include "AnimationFilm.h"
#include <assert.h>

AnimationFilm::AnimationFilm(SDL_Surface* bitmap, const vector<Rect> boxes, string &id) {
	this->id = id;
	this->bitmap = bitmap;
	this->boxes = boxes;
};

byte AnimationFilm::GetTotalFrames(void) const {
	return (byte)boxes.size()-1;
};

SDL_Surface* AnimationFilm::GetBitmap(void) const {
	return bitmap;
};

string AnimationFilm::GetId(void) const {
	return id;
};

Rect AnimationFilm::GetFrameBox(byte frameNo) const {
	assert(boxes.size() >= frameNo);//Check if this is working properly
	return boxes[frameNo];
};

void AnimationFilm::DisplayFrame(SDL_Surface &d, const Point &at, byte frameNo, int w, int h) const {
	Rect rect = { at.x,at.y,w,h };
	SDL_BlitScaled(bitmap, &GetFrameBox(frameNo), &d, &rect);
};


void AnimationFilm::DisplayFrame(SDL_Surface &d, Rect rect) const {
	//The rectangle were we are going to display the sprite size and coordinates
	SDL_BlitScaled(bitmap, 0, &d, &rect);
};
