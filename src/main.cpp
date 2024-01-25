#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "utils/camera.h"
#include "utils/vec3.h"

int main() {
	hittable_list world;

	world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(point3(0, -1, -1), 0.8));
	world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;

	cam.render(world);
}