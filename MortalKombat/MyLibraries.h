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

#define PUNCH_DMG 0.0002f
#define KICK_DMG 0.0004f
#define UPPERCUT_DMG 0.0006f

enum SpriteTypes { INTRO_SPRITE,FIGHTER, PROJECTILE};//Here ill add more 

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

#define GAME_TIMER 90

/*
*  ANIMATION DELAYS
*/

#define PUNCH_DELAY 100
#define LOW_PUNCH_DELAY 100
#define HIGH_PUNCH_DELAY 60
#define KICK_DELAY 100
#define LOW_KICK_DELAY 100
#define HIGH_KICK_DELAY 100
#define JUMP_DELAY 180
#define MOVE_DELAY 50
#define DUCK_DELAY 180
#define BLOCK_DELAY 100
#define STILL_BLOCK_DELAY 180
#define LOW_BLOCK_DELAY 180
#define SUBZERO_STANCE_DELAY 75
#define SCORPION_STANCE_DELAY 100
#define DOWNFLIP_DELAY 70
#define UPFLIP_DELAY 70
#define WIN_DELAY 180
#define LOSE_DELAY 250

/*
* INFLICTION ANIMATION DELAYS
*/

#define SINGLE_HIT_DELAY 50
#define DOWN_HIT_DELAY 100

/*
* DELAY TIMERS
*/
#define HIT_PUNCH_DELAY 350
#define HIT_KICK_DELAY 850

#define HIT_LOW_PUNCH_DELAY 650
#define HIT_LOW_KICK_DELAY 250

/*
* ANIMATION MOVEMENTS
*/
#define PUNCH_DIST 7
#define HIGH_PUNCH_DIST 25
#define KICK_DIST 17
#define HIGH_KICK_DIST 40
#define JUMP_DIST 120
#define MOVE_DIST 12
#define DOWN_FLIP_DIST 30
#define UP_FLIP_DIST 30


#endif
