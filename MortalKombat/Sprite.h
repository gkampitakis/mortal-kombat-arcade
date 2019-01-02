#ifndef Sprite_H
#define Sprite_H

#include "MyLibraries.h"
#include "AnimationFilm.h"

class Sprite {
private:
	bool				side;
	byte				frameNo;
	Rect				frameBox;
	Point				position;
	bool				visible;
	unsigned			type;
	AnimationFilm*		currFilm;
	Point				enemyPos;
public:
	Sprite(Point position,AnimationFilm* film, unsigned type,bool side);
	Sprite(Point position, AnimationFilm* film, unsigned type);
	~Sprite();

	Point GetEnemy(void) const;
	void SetEnemy(Point enemy);
	void SetFrame(byte i);
	byte GetFrame(void) const;
	void SetVisibility(bool v);
	bool IsVisible(void) const;
	bool CollisionCheck(Sprite* s);
	void Move(Point x);
	void Display(SDL_Surface &dest, int width, int height);
	void DisplayUnique(SDL_Surface &dest, int w,int h);
	unsigned GetType(void) const;
	AnimationFilm* getFilm(void);
	void SetNewFilm(AnimationFilm* film);
	byte GetFrameNo(void) const;
	void SetY(int y);
	void SetX(int x);
	Point GetPosition(void) const;
	void DisplayCamera(SDL_Surface &dest, int width, int height, Rect &camera);
};

#endif