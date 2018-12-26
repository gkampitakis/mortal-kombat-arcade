#include "Sprite.h"
#include <assert.h>

void Sprite::SetVisibility(bool v) {
	visible = v;
};

void Sprite::SetFrame(byte i) {//HERE BUG
	if (i != frameNo) {
		assert(i <= currFilm->GetTotalFrames());
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
	//For sure need fix :P
	return true;
};

void Sprite::Move(Point x) {
	this->position.x = this->position.x+x.x;
	this->position.y = this->position.y + x.y;
};

void Sprite::Display(SDL_Surface &dest, int width, int height) {
	if (visible) {
		currFilm->DisplayFrame(dest,position,frameNo, width,height);
	}
};

void Sprite::DisplayUnique(SDL_Surface &dest,int width,int height) {
	if (visible) {
		Rect display = { position.x,position.y,width,height };
		currFilm->DisplayFrame(dest, display); 
	}
};


Sprite::Sprite(Point _position, AnimationFilm* film,unsigned type) {
	position = _position;
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
};

void Sprite::SetNewFilm(AnimationFilm* film) {
	currFilm = film;
	frameNo = film->GetTotalFrames();
};

byte Sprite::GetFrameNo(void) const {
	return  frameNo;
};

void Sprite::SetY(int y) {
	position.y = y;
};

Point Sprite::GetPosition(void) const {
	return position;
};