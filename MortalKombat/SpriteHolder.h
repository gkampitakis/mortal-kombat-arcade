#include "Sprite.h"

typedef list<Sprite*> SpriteList;

class SpriteHolder {
private:
	static SpriteHolder* holder;
protected:
	typedef map<unsigned, SpriteList> SpritesByType;
	SpritesByType sprites;
public:
	SpriteHolder(void) {};
	~SpriteHolder() { CleanUp(); };

	static SpriteHolder* Get(void);
	void Add(Sprite* s);
	void Remove(Sprite* s);
	SpriteList GetSprites(unsigned type)const;
	void CleanUp();
};