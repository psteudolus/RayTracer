#ifndef MATERIALH
#define MATERIALH

#include "Hitable.h"

Vec3  randomInUnitSphere() {
	Vec3 p;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	auto rand = std::bind(distribution, mt);
	do {
		p = 2.0 * Vec3(rand(), rand(), rand()) - Vec3(1, 1, 1);
	} while (p.squaredLength() >= 1.0);
	return p;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted) {
	Vec3 uv = unitVector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - niOverNt * niOverNt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float reflectiveIndex) {
	float r0 = (1 - reflectiveIndex) / (1 + reflectiveIndex);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Material {
public:
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& a) : albedo(a) {}
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	Vec3 albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3& a, float f) : albedo(a) {
		if (f < 1) fuzz = f;
		else fuzz = 1;
	}
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	Vec3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float reflectiveIndexIn) : reflectiveIndex(reflectiveIndexIn) {}
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 outwardNormal;
		Vec3 reflected = reflect(rayIn.direction(), rec.normal);
		float niOverNt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflectiveProbability;
		float cosine;
		if (dot(rayIn.direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			niOverNt = reflectiveIndex;
			cosine = reflectiveIndex * dot(rayIn.direction(), rec.normal) / rayIn.direction().length();
		}
		else {
			outwardNormal = rec.normal;
			niOverNt = 1.0 / reflectiveIndex;
			cosine = -dot(rayIn.direction(), rec.normal) / rayIn.direction().length();
		}
		if (refract(rayIn.direction(), outwardNormal, niOverNt, refracted)) {
			reflectiveProbability = schlick(cosine, reflectiveIndex);
		}
		else {
			reflectiveProbability = 1.0;
		}
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> distribution(0.0, 1.0);
		auto rand = std::bind(distribution, mt);
		if (rand() < reflectiveProbability) {
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}
	float reflectiveIndex;
};
#endif // !MATERIALH
