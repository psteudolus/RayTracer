#ifndef MATERIALH
#define MATERIALH

#include "Hitable.h"

class Material {
public:
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};
#endif // !MATERIALH
