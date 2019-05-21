// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "RayTracer.hpp"

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


void parallelTrace(int nx, int ny, Camera& cam, Hitable*& world, int ns, Vec3** image)
{
	std::size_t max = nx * ny;
	std::size_t cores = std::thread::hardware_concurrency();
	volatile std::atomic<std::size_t> count(0);
	std::vector<std::future<void>> futureVector;

	while (cores--) {
		futureVector.emplace_back(
			std::async([=, &cam, &world, &count]() {
				while (true) {
					std::size_t index = count++;
					if (index >= max) {
						break;
					}
					std::size_t x = index % nx;
					std::size_t y = index / nx;
					Vec3 col(0, 0, 0);
					for (int s = 0; s < ns; ++s) {
						std::random_device rd;
						std::mt19937 mt(rd());
						std::uniform_real_distribution<float> distribution(0.0, 1.0);
						auto rand = std::bind(distribution, mt);
						float u = float(x + rand()) / float(nx);
						float v = float(y + rand()) / float(ny);

						Ray r(cam.getRay(u, v));
						Vec3 p(r.pointAtParameter(2.0));
						col += color(r, world, 0);
						//std::cout << index << std::endl;
					}
					col /= float(ns);
					col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
					image[index] = new Vec3(col);
				}
				})
		);
	}
}


int main()
{
	std::ofstream output;
	output.open("helloraytracerrefactored.ppm");
	int nx = 233;
	int ny = 100;
	int ns = 100;
	
	std::vector<Vec3> pixels;
	Vec3** image = new Vec3 * [nx * ny];

	float R = cos(getPI() / 4);
	
	Hitable * list[4];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
	list[1] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[2] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[3] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
	//list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	Hitable* world = new HitableList(list, 4);
	
	
	//Hitable* world = randomScene();
	Vec3 lookFrom(11, 1.8, 2.8);
	Vec3 lookAt(-1, 0.3, -1);
	Vec3 upVector(0, 1, 0);
	float distanceToFocus = (lookFrom - lookAt).length();
	float aperture = 0.1;
	float FOV = 20;
	float aspectRatio = float(nx) / float(ny);

	Camera cam(lookFrom, lookAt, upVector, FOV, aspectRatio, aperture, distanceToFocus);

	parallelTrace(nx, ny, cam, world, ns, image);

	output << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; --j) {
		for (int i = 0; i < nx; ++i) {

			int ir = int(255.99 * image[i + nx * j]->e[0]);
			int ig = int(255.99 * image[i + nx * j]->e[1]);
			int ib = int(255.99 * image[i + nx * j]->e[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}

	/*
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
			pixels.push_back(col);
			//std::cout << "*";
		}
		std::cout << j << std::endl;
	}

	output << "P3\n" << nx << " " << ny << "\n255\n";
	for (auto it = pixels.begin(); it != pixels.end(); ++it) {

		int ir = int(255.99 * it->e[0]);
		int ig = int(255.99 * it->e[1]);
		int ib = int(255.99 * it->e[2]);
		output << ir << " " << ig << " " << ib << "\n";
	}*/
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
