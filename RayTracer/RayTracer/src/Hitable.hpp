#ifndef HITABLEHPP
#define HITABLEHPP

#include "Ray.hpp"

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
};
#endif // !HITABLEHPP
