#include "Sprite.h"
#include <assert.h>

void Sprite::SetVisibility(bool v) {
	visible = v;
};

void Sprite::SetFrame(byte i) {
	if (i != frameNo) {
		assert(i < currFilm->GetTotalFrames());
		frameBox = currFilm->GetFrameBox(frameNo = i);
	}
};

byte Sprite::GetFrame(void) const{
	return frameNo;
};

bool Sprite::IsVisible(void) const {
	return visible;
};

bool Sprite::CollisionCheck(Sprite* s) {
	//might needs fix 
	return true;
};

void Sprite::Move(Point x) {
	this->position = x;
};

void Sprite::Display(SDL_Surface &dest) {
	if (visible) {
		currFilm->DisplayFrame(dest,position,frameNo);//this might need fix 
	}
};

void Sprite::DisplayUnique(SDL_Surface &dest,int width,int height) {
	if (visible) {
		Rect display = { position.x,position.y,width,height };
		currFilm->DisplayFrame(dest, display);//this might need fix 
	}
};


Sprite::Sprite(Point position, AnimationFilm* film,unsigned type) {
	this->position = position;
	this->currFilm = film;
	SetVisibility(true);
	SetFrame(0);
	this->frameNo = currFilm->GetTotalFrames();
	this->type = type;
};

Sprite::~Sprite() {};

unsigned Sprite::GetType(void) const {
	return type;
}

AnimationFilm* Sprite::getFilm(void) {
	return currFilm;
}