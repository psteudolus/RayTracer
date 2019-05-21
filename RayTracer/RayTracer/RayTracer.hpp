
#include "float.h"
#include <iostream>
#include <fstream>
#include <random>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <future>
#include "src/Sphere.hpp"
#include "src/HitableList.hpp"
#include "src/Camera.hpp"
#include "src/Material.hpp"

void parallelTrace(int nx, int ny, Camera& cam, Hitable*& world, int ns, Vec3** image);
