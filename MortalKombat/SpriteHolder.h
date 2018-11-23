#include <Sprite.h>

class SpriteHolder {
private:
	typedef list<Sprite*> SpriteList;
protected:
	typedef map<unsigned, SpriteList> SpritesByType;
	SpritesByType sprites;
public:
	SpriteHolder(void);
	~SpriteHolder() { CleanUp(); };

	static SpriteHolder& Get(void);
	void Add(Sprite* s);
	void Remove(Sprite* s);
	void GetSprites(unsigned type, SpriteList* result)const;
	void CleanUp();
};