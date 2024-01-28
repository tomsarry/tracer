#include "scenes.h"

#include "materials/dielectric.h"
#include "materials/iridescent.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"

namespace scenes::book_one {

void construct_world(hittable_list &world) {
	auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));

	world.add(
		std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(
				a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(
						std::make_shared<sphere>(center, 0.2, sphere_material));
				} else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(
						std::make_shared<sphere>(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(
						std::make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
}

void construct_camera(camera &cam, int samples_per_pixel, int max_depth) {
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 200;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 20;
	cam.look_from = point3(13, 2, 3);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;
}

void end(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	construct_world(world);
	construct_camera(cam, samples_per_pixel, max_depth);
}

void performance_analysis(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<iridescent>(1.5);
	auto material_right = std::make_shared<iridescent>(false);

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

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1000;
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.vertical_fov = 30;
	cam.look_from = point3(-2, 2, 1);
	cam.look_at = point3(0, 0, -1);
	cam.vertical_up = vec3(0, 1, 0);
}

void simple_scene_rrt(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<iridescent>(true);
	auto material_right = std::make_shared<iridescent>(false);

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

	cam.aspect_ratio = 1;
	cam.image_width = 200;
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.vertical_fov = 30;
	cam.look_from = point3(-2, 0, 1);
	cam.look_at = point3(0, 0, -1);
	cam.vertical_up = vec3(0, 1, 0);
}

}  // namespace scenes::book_one