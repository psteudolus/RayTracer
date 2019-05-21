#ifndef RAYHPP
#define RAYHPP

#include "Vec3.hpp"

class Ray {
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b);
	Vec3 origin() const;
	Vec3 direction() const;
	Vec3 pointAtParameter(float t) const;

	Vec3 A;
	Vec3 B;
};
#endif // !RAYHPP