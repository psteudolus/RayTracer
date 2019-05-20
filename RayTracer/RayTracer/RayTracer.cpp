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
#include "Material.h"

Vec3 color(const Ray& r, Hitable* world, int depth) {
	HitRecord rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return Vec3(0, 0, 0);
		}
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
	int nx = 699;
	int ny = 300;
	int ns = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable* list[4];
	float R = cos(getPI() / 4);
	//list[0] = new Sphere(Vec3(-R, 0, -1), R, new Lambertian(Vec3(0, 0, 1)));
	//list[1] = new Sphere(Vec3(R, 0, -1), R, new Lambertian(Vec3(1, 0, 0)));
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.5));
	list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	//list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	Hitable* world = new HitableList(list, 4);
	Camera cam(Vec3(-2, 2, 1), Vec3(0, 0, -1), Vec3(0, 1, 0), 40, float(nx) / float(ny));

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
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			//std::cout << "*";
			output << ir << " " << ig << " " << ib << "\n";
		}
		std::cout << j << std::endl;
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