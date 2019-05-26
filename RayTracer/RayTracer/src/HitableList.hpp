#ifndef HITABLELISTHPP
#define HITABLELISTHPP

#include "Hitable.hpp"
#include <vector>

class HitableList : public Hitable {
public: 
	HitableList();
	HitableList(std::vector<std::shared_ptr<Hitable>> l, int n);

	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

	//Hitable** list;
	std::vector<std::shared_ptr<Hitable>> list;
	int listSize;
};
#endif // !HITABLELISTHPP