#ifndef MATERIALHPP
#define MATERIALHPP

#include <random>
#include <functional>
#include "Hitable.hpp"

Vec3 randomInUnitSphere();

Vec3 reflect(const Vec3& v, const Vec3& n);

bool refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted);

float schlick(float cosine, float reflectiveIndex);

class Material {
public:
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& a);
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const;

	Vec3 albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3& a, float f);
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const;

	Vec3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float reflectiveIndexIn);
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const;
	float reflectiveIndex;
};
#endif // !MATERIALHPP
