#include "scenes.h"

#include <memory>

#include "materials/dielectric.h"
#include "materials/diffuse_light.h"
#include "materials/iridescent.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/scratched_metal.h"
#include "objects/hittable_list.h"
#include "objects/quad.h"
#include "objects/sphere.h"
#include "textures/checker_texture.h"
#include "textures/image_texture.h"
#include "textures/scratch_texture.h"
#include "utils/point2.h"
#include "utils/vec3.h"

namespace {
color magenta{1, 0, 1};
color white{1, 1, 1};
color orange{1, 0.64, 0};
}  // namespace

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
	auto material_metal = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.8);

	world.add(std::make_shared<sphere>(
		point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(
		std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(
		std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(
		std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
	world.add(
		std::make_shared<sphere>(point3(2.0, 0.0, -1.0), 0.5, material_metal));

	cam.aspect_ratio = 1;
	cam.image_width = 200;
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.vertical_fov = 30;
	cam.look_from = point3(-2, 2, 1);
	cam.look_at = point3(0, 0, -1);
	cam.vertical_up = vec3(0, 1, 0);
}

void video(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<iridescent>(true);

	world.add(std::make_shared<sphere>(
		point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(
		std::make_shared<sphere>(point3(0.0, 0.0, 0), 0.1, material_center));
	world.add(
		std::make_shared<sphere>(point3(-.2, 0.0, 0), 0.1, material_left));

	cam.aspect_ratio = 1;
	cam.image_width = 200;
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.vertical_fov = 45;
	cam.look_from = point3(-2, 2, 1);
	cam.look_at = point3(-0.1, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);
}

}  // namespace scenes::book_one

namespace scenes::book_two {
void bouncing_spheres(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
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
					auto center2 = center + vec3(0, random_double(0, 0.5), 0);
					world.add(std::make_shared<sphere>(
						center, center2, 0.2, sphere_material));
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

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 20;
	cam.look_from = point3(13, 2, 3);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0.02;
	cam.focus_dist = 10.0;
}

void two_spheres(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	auto checker = std::make_shared<checker_texture>(
		0.8, color(.2, .3, .1), color(.9, .9, .9));

	world.add(std::make_shared<sphere>(
		point3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
	world.add(std::make_shared<sphere>(
		point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.vertical_fov = 20;
	cam.look_from = point3(13, 2, 3);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void earth(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	auto earth_texture = std::make_shared<image_texture>("earthmap.jpg");
	auto earth_surface = std::make_shared<lambertian>(earth_texture);
	auto globe = std::make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
	world.add(globe);

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);

	cam.vertical_fov = 20;
	cam.look_from = point3(0, 0, 12);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void quads(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	auto left_red = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));
	auto lower_teal = std::make_shared<lambertian>(color(0.2, 0.8, 0.8));

	// Quads
	world.add(std::make_shared<quad>(
		point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(std::make_shared<quad>(
		point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
	world.add(std::make_shared<quad>(
		point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
	world.add(std::make_shared<quad>(
		point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
	world.add(std::make_shared<quad>(
		point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

	cam.aspect_ratio = 1.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);

	cam.vertical_fov = 80;
	cam.look_from = point3(0, 0, 9);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

}  // namespace scenes::book_two

namespace scenes::scratches {
void single(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	auto texture = std::make_shared<image_texture>("scratch_3.png");
	auto surface = std::make_shared<lambertian>(texture);
	auto left_red = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));

	// Quads
	world.add(std::make_shared<quad>(
		point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(std::make_shared<quad>(
		point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
	world.add(std::make_shared<quad>(
		point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
	world.add(std::make_shared<quad>(
		point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
	world.add(std::make_shared<quad>(
		point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), surface));

	cam.aspect_ratio = 1.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 80;
	cam.look_from = point3(0, 0, 9);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void monkey(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	std::vector<placement_info> placements;
	// placements.emplace_back(16, 2, point2(.3, .2));
	// placements.emplace_back(16, 2, point2(.3, .3));
	placements.emplace_back(16, 2, point2(.3, .4));
	placements.emplace_back(16, 2, point2(.3, .0));

	auto texture = std::make_shared<image_texture>(
		"scratch_3.png", orange, magenta, placements);
	auto surface = std::make_shared<lambertian>(texture);

	// Quads
	world.add(std::make_shared<quad>(
		point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), surface));

	cam.aspect_ratio = 1.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 60;
	cam.look_from = point3(0, 0, 6);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void close(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	auto left_red = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));

	scratch s1{point2(0.1, 0.1), point2(0.5, 0.2), 0.001};
	scratch s2{point2(0.1, 0.2), point2(0.5, 0.3), 0.001};
	std::vector scratches{s1, s2};
	auto scratch_tex =
		std::make_shared<scratch_texture>(scratches, white, magenta);
	auto scratch_texture = std::make_shared<lambertian>(scratch_tex);

	// Quads
	world.add(std::make_shared<quad>(
		point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(std::make_shared<quad>(
		point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
	world.add(std::make_shared<quad>(
		point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
	world.add(std::make_shared<quad>(
		point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
	world.add(std::make_shared<quad>(
		point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), scratch_texture));

	cam.aspect_ratio = 1.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 40;
	cam.look_from = point3(0, -1.5, 6);
	cam.look_at = point3(-1.5, -3, 4);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void far(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	auto left_red = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));

	scratch s1{point2(0.1, 0.1), point2(0.5, 0.2), 0.001};
	scratch s2{point2(0.1, 0.2), point2(0.5, 0.3), 0.001};
	std::vector scratches{s1, s2};

	auto scratch_tex_white =
		std::make_shared<scratch_texture>(scratches, white, magenta);
	auto scratch_texture_white =
		std::make_shared<lambertian>(scratch_tex_white);

	auto scratch_tex_orange =
		std::make_shared<scratch_texture>(scratches, orange, magenta);
	auto scratch_texture_orange =
		std::make_shared<lambertian>(scratch_tex_orange);

	// Quads
	world.add(std::make_shared<quad>(
		point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(std::make_shared<quad>(
		point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
	world.add(std::make_shared<quad>(
		point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
	world.add(std::make_shared<quad>(
		point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4),
		scratch_texture_orange));
	world.add(std::make_shared<quad>(
		point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4),
		scratch_texture_white));

	cam.aspect_ratio = 1.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 80;
	cam.look_from = point3(0, 0, 9);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void complex(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	auto red = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto blue = std::make_shared<lambertian>(color(.2, 0.2, 1));
	auto met = std::make_shared<metal>(color(.8, .8, .8), .1);

	auto checker = std::make_shared<checker_texture>(
		0.2, color(.1, .1, .1), color(.9, .9, .9));

	// scale2: 1024 * 4 = 4k
	// scale2: 1024 * 9 = 9k
	// scale4: 1024 * 16 = 16k

	// 2048 * 4 = 8k
	// 2048 * 9 = 18k
	placement_info info{4, 4, point2(.0, .0)};
	auto repeat = true;

	auto smet = std::make_shared<scratched_metal>(
		color(.4, .4, 1), .1, "1024_normal.png", info, repeat);

	// back
	world.add(std::make_shared<quad>(
		point3(-2, -2, 5), vec3(4, 0, 0), vec3(0, 4, 0),
		std::make_shared<lambertian>(checker)));
	// bottom
	world.add(std::make_shared<quad>(
		point3(-2, -2, 1), vec3(0, 0, 4), vec3(4, 0, 0),
		std::make_shared<lambertian>(checker)));

	// scratched metal
	world.add(std::make_shared<quad>(
		point3(.25, -.5, -.25), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));

	auto light = std::make_shared<diffuse_light>(color(10, 10, 10));

	// right light
	world.add(std::make_shared<sphere>(point3(-0.35, -.3, 0.1), .1, light));

	// left light
	world.add(std::make_shared<sphere>(point3(0.35, -.3, -0.1), .1, light));

	cam.aspect_ratio = 1.0;
	cam.image_width = 600;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 40;
	cam.look_from = point3(0, -.2, -1.);
	cam.look_at = point3(0, -.5, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}

void complex2(
	hittable_list &world, camera &cam, int samples_per_pixel, int max_depth) {
	// Materials
	auto blue = std::make_shared<lambertian>(color(.2, 0.2, 1));
	auto met = std::make_shared<metal>(color(.8, .8, .8), .1);

	// scale2: 1024 * 4 = 4k
	// scale2: 1024 * 9 = 9k
	// scale4: 1024 * 16 = 16k

	// 2048 * 4 = 8k
	// 2048 * 9 = 18k
	placement_info info{4, 4, point2(.0, .0)};
	auto repeat = true;

	auto smet = std::make_shared<scratched_metal>(
		color(.4, .4, 1), .1, "1024_normal.png", info, repeat);

	// scratched metal
	// front
	world.add(std::make_shared<quad>(
		point3(.25, -.5, -1), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));
	world.add(std::make_shared<quad>(
		point3(.25, -.5, -.50), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));
	world.add(std::make_shared<quad>(
		point3(.25, -.5, 0), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));
	world.add(std::make_shared<quad>(
		point3(.25, -.5, .50), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));
	world.add(std::make_shared<quad>(
		point3(.25, -.5, 1), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));
	world.add(std::make_shared<quad>(
		point3(.25, -.5, 1.50), vec3(-.5, 0, 0), vec3(0, 0, .5), smet));
	// back

	auto light = std::make_shared<diffuse_light>(color(10, 10, 10));

	world.add(std::make_shared<sphere>(point3(-0.45, -.3, 0), .1, light));
	world.add(std::make_shared<sphere>(point3(0.45, -.3, 0), .1, light));

	world.add(std::make_shared<sphere>(point3(-0.55, -.3, 1.5), .1, light));
	world.add(std::make_shared<sphere>(point3(0.55, -.3, 1.5), .1, light));

	cam.aspect_ratio = 1.0;
	cam.image_width = 600;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;

	cam.vertical_fov = 40;
	cam.look_from = point3(0, -.2, -1.);
	cam.look_at = point3(0, -.5, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0;
}
}  // namespace scenes::scratches