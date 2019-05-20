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

Hitable* randomScene() {
	int n = 500;
	Hitable** list = new Hitable* [n + 1];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<float> distribution(0.0, 1.0);
			auto rand = std::bind(distribution, mt);

			float chooseMat = rand();
			Vec3 center(a + 0.9 * rand(), 0.2, b + 0.9 * rand());
			if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
				if (chooseMat < 0.8) {			//diffuse
					list[i++] = new Sphere(center, 0.2, new Lambertian(Vec3(rand() * rand(), rand() * rand(), rand() * rand())));
				}
				else if (chooseMat < 0.95) {	//metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vec3(0.5 * (1 + rand()), 0.5 * (1 + rand()), 0.5 * (1 + rand())), 0.5 * rand()));
				}
				else {	//glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(list, i);
}

int main()
{
	std::ofstream output;
	output.open("helloraytracer.ppm");
	int nx = 466;
	int ny = 200;
	int ns = 10;
	
	//Vec3** pixels = new Vec3 * [nx * ny];
	
	output << "P3\n" << nx << " " << ny << "\n255\n";

	float R = cos(getPI() / 4);
	/*
	Hitable * list[4];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
	list[1] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[2] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[3] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
	//list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	Hitable* world = new HitableList(list, 4);
	*/
	
	Hitable* world = randomScene();
	Vec3 lookFrom(11, 1.8, 2.8);
	Vec3 lookAt(-1, 0.3, -1);
	Vec3 upVector(0, 1, 0);
	float distanceToFocus = (lookFrom - lookAt).length();
	float aperture = 0.1;
	float FOV = 20;
	float aspectRatio = float(nx) / float(ny);

	Camera cam(lookFrom, lookAt, upVector, FOV, aspectRatio, aperture, distanceToFocus);
	//int pixeli = 0;
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
			//pixels[pixeli++] = new Vec3(col);
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
