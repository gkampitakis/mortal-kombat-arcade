#ifndef Animation_HEADER
#define Animation_HEADER

#include "MyLibraries.h"

class Animation {
	animid_t id;
public:
	animid_t GetId(void) { return id; };
	virtual Animation* Clone(animid_t newId) const = 0;
	Animation(animid_t _id) :id(_id) { }
	Animation() {};
	virtual ~Animation() {}
};

#endif