#ifndef SPHEREH
#define SPHEREH

#include "Hitable.hpp"

class Sphere : public Hitable {
public:
	Sphere();
	Sphere(Vec3 cen, float r, std::shared_ptr<Material> matPtrIn);
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

	Vec3 center;
	float radius;
	std::shared_ptr<Material> matPtr;
};
#endif // !SPHEREH
