#include "materials/dielectric.h"
#include "materials/iridescent.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "utils/camera.h"
#include "utils/vec3.h"

int main() {
	hittable_list world;

	auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<iridescent>();
	auto material_right = std::make_shared<iridescent>(true);

	world.add(std::make_shared<sphere>(
		point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(
		std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(
		std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(
		std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
	world.add(
		std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.vertical_fov = 20;
	cam.look_from = point3(-2, 2, 1);
	cam.look_at = point3(0, 0, -1);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 10.0;
	cam.focus_dist = 3.4;

	cam.render(world);
}