#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

class Material;

struct HitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	Material* matPtr;
};

class Hitable {
public:
	Hitable();
	~Hitable();
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
private:

};

Hitable::Hitable() {
}

Hitable::~Hitable() {
}
#endif // !HITABLEH
