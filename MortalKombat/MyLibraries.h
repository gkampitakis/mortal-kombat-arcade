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
#include <set>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using json = nlohmann::json;

typedef unsigned char byte;
typedef SDL_Rect Rect;
typedef SDL_Point Point;

typedef signed char offset_t;
typedef unsigned short delay_t;
typedef unsigned short animid_t;
typedef unsigned long timestamp_t;
typedef unsigned char frame_t;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 920 

#define STAGE_WIDTH 2008
#define STAGE_HEIGHT 920

enum SpriteTypes { INTRO_SPRITE,FIGHTER };//Here ill add more 

enum animatorstate_t {
	ANIMATOR_FINISHED = 0,
	ANIMATOR_RUNNING = 1,
	ANIMATOR_STOPPED = 2
};

enum Game_State { 
	INTRO,
	DISCLAIMER, 
	INGAME, 
	FINISH
};//This states might change or add new states

#endif
