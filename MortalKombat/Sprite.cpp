#include "Sprite.h"
#include <assert.h>

void Sprite::SetVisibility(bool v) {
	visible = v;
};

void Sprite::SetFrame(byte i) {
	if (i != frameNo) {
		assert(i <= currFilm->GetTotalFrames());
		frameBox = currFilm->GetFrameBox(frameNo = i);
	}
};

byte Sprite::GetFrame(void) const {
	return frameNo;
};

bool Sprite::IsVisible(void) const {
	return visible;
};

bool Sprite::CollisionCheck(Sprite* s) {
	//here we need extra check when special combos are fixed 104 SAFE_DISTANCE
	if (abs(position.x - s->GetPosition().x) > 124) return false;
	else return true;
};

void Sprite::Move(Point x) {
	this->position.x = this->position.x + x.x;
	this->position.y = this->position.y + x.y;

	if (side) {
		if (x.x < 0) {//1100 define max dist
			if (enemyPos.x - position.x > 1090) {
				this->position.x = this->position.x - x.x;
			}
		}
		if (enemyPos.x - position.x < 80) {
			this->position.x = this->position.x - x.x;
		}
		if (position.x - 50 < 0) {
			this->position.x = this->position.x - x.x;
		}
	}
	else {
		if (x.x > 0) {
			if (position.x - enemyPos.x > 1090) {
				this->position.x = this->position.x - x.x;
			}
		}
		if (position.x - enemyPos.x < 80) {
			this->position.x = this->position.x - x.x;
		}
		if (position.x + 180>STAGE_WIDTH) {
			this->position.x = this->position.x - x.x;
		}
	}
};

void Sprite::Display(SDL_Surface &dest, int width, int height) {
	if (visible) {
		currFilm->DisplayFrame(dest, position, frameNo, width, height);
	}
};

void Sprite::DisplayCamera(SDL_Surface &dest, int width, int height,Rect &camera) {
	if (visible) {
		Point temp = { position.x - camera.x,position.y - camera.y };
		currFilm->DisplayFrame(dest, temp, frameNo, width, height);
	}
};

void Sprite::DisplayUnique(SDL_Surface &dest, int width, int height) {
	if (visible) {
		Rect display = { position.x,position.y,width,height };
		currFilm->DisplayFrame(dest, display);
	}
};

void Sprite::SetEnemy(Point enemy) {
	enemyPos = enemy;
}

Sprite::Sprite(Point _position, AnimationFilm* film, unsigned type, bool _side) {
	side = _side;
	position = _position;
	this->currFilm = film;
	SetVisibility(true);
	SetFrame(0);
	this->frameNo = currFilm->GetTotalFrames();
	this->type = type;
};

Sprite::Sprite(Point _position, AnimationFilm* film, unsigned type) {
	side = false;
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

void Sprite::SetX(int x) {
	position.x = x;
};

Point Sprite::GetPosition(void) const {
	return position;
}