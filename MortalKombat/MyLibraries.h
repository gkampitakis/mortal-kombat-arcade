#ifndef MyLibrary_H
#define MyLibrary_H


#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <list>

using namespace std;

typedef unsigned char byte;
typedef SDL_Rect Rect;
typedef SDL_Point Point;

//maybe add the sdl_surface as bitmap but we ll see
#define SCREEN_WIDTH 960//might need fix
#define SCREEN_HEIGHT 720

enum SpriteTypes { MenuSprite };//Here ill add more 


#endif