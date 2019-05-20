#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"
double getPI() {
	double pi;
	__asm {
		fldpi
		fstp pi
	}
	return pi;
}

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float verticalFov, float aspect) {
		Vec3 u, v, w;
		float theta = verticalFov * getPI() / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vup, w));
		v = cross(w, u);
		lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}
	Ray getRay(float s, float t) {
		return Ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin);
	}

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
};
#endif // !CAMERAH
