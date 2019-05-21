#include "Camera.hpp"

double getPI() {
	double pi;
	__asm {
		fldpi
		fstp pi
	}
	return pi;
}

Vec3 randomInUnitDisk() {
	Vec3 p;
	do {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> distribution(0.0, 1.0);
		auto rand = std::bind(distribution, mt);

		p = 2.0 * Vec3(rand(), rand(), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

Camera::Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float verticalFov, float aspect, float aperture, float focusDist) {
	lensRadius = aperture / 2;
	float theta = verticalFov * getPI() / 180;
	float halfHeight = tan(theta / 2);
	float halfWidth = aspect * halfHeight;
	origin = lookFrom;
	w = unitVector(lookFrom - lookAt);
	u = unitVector(cross(vup, w));
	v = cross(w, u);
	lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
	horizontal = 2 * halfWidth * focusDist * u;
	vertical = 2 * halfHeight * focusDist * v;
}
Ray Camera::getRay(float s, float t) {
	Vec3 rd = lensRadius * randomInUnitDisk();
	Vec3 offset = u * rd.x() + v * rd.y();
	return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
}