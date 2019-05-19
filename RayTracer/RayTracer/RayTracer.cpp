// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <random>
#include <functional>
#include "sphere.h"
#include "HitableList.h"
#include "float.h"
#include "Camera.h"

Vec3  randomInUnitSphere() {
	Vec3 p;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	auto rand = std::bind(distribution, mt);
	do {
		p = 2.0 * Vec3(rand(), rand(), rand()) - Vec3(1,1,1);
	} while (p.squaredLength() >= 1.0);
	return p;
}

Vec3 color(const Ray& r, Hitable* world) {
	HitRecord rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Vec3 target = rec.p + rec.normal + randomInUnitSphere();
		return 0.5* color(Ray(rec.p, target - rec.p), world);
	}
	else {
		Vec3 unitDirection = unitVector(r.direction());
		float t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	std::ofstream output;
	output.open("helloraytracer.ppm");
	int nx = 2000;
	int ny = 1000;
	int ns = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable* list[2];
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100);
	Hitable* world = new HitableList(list, 2);
	Camera cam;

	for (int j = ny - 1; j >= 0; --j) {
		for (int i = 0; i < nx; ++i) {
			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; ++s) {
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<float> distribution(0.0, 1.0);
				auto rand = std::bind(distribution, mt);
				float u = float(i + rand()) / float(nx);
				float v = float(j + rand()) / float(ny);

				Ray r(cam.getRay(u, v));
				Vec3 p(r.pointAtParameter(2.0));
				col += color(r, world);
			}
			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			output << ir << " " << ig << " " << ib << "\n";
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
