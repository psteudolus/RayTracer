#include "HitableList.hpp"


HitableList::HitableList() {}

HitableList::HitableList(Hitable** l, int n) : list(l), listSize(n) {}

bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;

	double closestSoFar = tMax;
	for (int i = 0; i < listSize; ++i) {
		if (list[i]->hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}