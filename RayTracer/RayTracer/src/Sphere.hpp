#ifndef SPHEREH
#define SPHEREH

#include "Hitable.hpp"

class Sphere : public Hitable {
public:
	Sphere();
	Sphere(Vec3 cen, float r, Material* matPtrIn);
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

	Vec3 center;
	float radius;
	Material* matPtr;
};
#endif // !SPHEREH
