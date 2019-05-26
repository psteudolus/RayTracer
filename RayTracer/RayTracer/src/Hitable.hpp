#ifndef HITABLEHPP
#define HITABLEHPP

#include "Ray.hpp"

class Material;

struct HitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	//raw pointer, but the question here is "is it worth it to move to a smart pointer here?" 
	//observer_ptr isn't part of the stl yet and in modern convention raw pointers in low level data structures for observation 
	//is still accepted practice
	std::shared_ptr<Material> matPtr;
};

class Hitable {
public:
	Hitable();
	~Hitable();
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};
#endif // !HITABLEHPP
