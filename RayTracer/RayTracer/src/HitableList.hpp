#ifndef HITABLELISTHPP
#define HITABLELISTHPP

#include "Hitable.hpp"

class HitableList : public Hitable {
public: 
	HitableList();
	HitableList(Hitable** l, int n);

	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

	Hitable** list;
	int listSize;
};
#endif // !HITABLELISTHPP