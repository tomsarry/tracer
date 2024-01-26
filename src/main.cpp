#include "materials/lambertian.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "utils/camera.h"
#include "utils/vec3.h"

int main() {
	hittable_list world;

	world.add(std::make_shared<sphere>(
		point3(0, 0, -1), 0.5, std::make_shared<lambertian>(color(1, 1, 0))));
	world.add(std::make_shared<sphere>(
		point3(0, -100.5, -1), 100,
		std::make_shared<lambertian>(color(0, 0, 1))));

	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 64;
	cam.max_depth = 50;

	cam.render(world);
}