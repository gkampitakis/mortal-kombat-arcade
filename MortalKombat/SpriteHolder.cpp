#include <SpriteHolder.h>

void SpriteHolder::CleanUp() {
	//for (SpriteList::const_iterator i = sprites.begin(); i != films.end(); i++) delete(i->second);
	//films.clear();
	//This neeeds fix 
};

SpriteHolder::SpriteHolder(void) {};

void SpriteHolder::Add(Sprite* s) {
	sprites[s->GetType()].push_back(s);//This also
};

void SpriteHolder::Remove(Sprite* s) {
	sprites[s->GetType()].remove(s);
};

void SpriteHolder::GetSprites(unsigned type, SpriteList* result) const {
	SpritesByType::const_iterator i = sprites.find(type);
	if (i != sprites.end()) *result = i->second;
	//This also Might needs fix
};
