#include "HitableList.hpp"


HitableList::HitableList() {}

HitableList::HitableList(std::vector<std::shared_ptr<Hitable>> l, int n) : list(l), listSize(n) {
	l.reserve(listSize);
}

bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;

	double closestSoFar = tMax;
	for (int i = 0; i < listSize; ++i) {
		if (list[i]->hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = std::move(tempRec);
		}
	}
	return hitAnything;
}