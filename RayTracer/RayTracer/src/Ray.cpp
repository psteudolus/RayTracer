#include "Ray.hpp"

Ray::Ray(const Vec3& a, const Vec3& b) { A = a; B = b; }
Vec3 Ray::origin() const { return A; }
Vec3 Ray::direction() const { return B; }
Vec3 Ray::pointAtParameter(float t) const { return A + t * B; }
