#include "SpriteHolder.h"

SpriteHolder* SpriteHolder::holder = new SpriteHolder();//singleton

SpriteHolder* SpriteHolder::Get(void) {
	return holder;
};

void SpriteHolder::Add(Sprite* s) {
	sprites[s->GetType()].push_back(s);
};

void SpriteHolder::Remove(Sprite* s) {
	sprites[s->GetType()].remove(s);
};

SpriteList SpriteHolder::GetSprites(unsigned type) const {
	SpritesByType::const_iterator i = sprites.find(type);
	return i != sprites.end() ? i->second : (SpriteList)NULL;
};

void SpriteHolder::CleanUp() {
	sprites.clear();
};