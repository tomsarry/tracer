#include <future>

#include "materials/dielectric.h"
#include "materials/iridescent.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "utils/camera.h"
#include "utils/color.h"
#include "utils/pixel_map.h"
#include "utils/vec3.h"

hittable_list construct_world() {
	hittable_list world;

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

	return world;
}

camera construct_camera() {
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.image_height = static_cast<int>(cam.image_width / cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;

	cam.samples_per_pixel = 16;
	cam.max_depth = 10;

	cam.vertical_fov = 20;
	cam.look_from = point3(13, 2, 3);
	cam.look_at = point3(0, 0, 0);
	cam.vertical_up = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	return cam;
}

int main() {
	auto world = construct_world();

	camera cam = construct_camera();

	const auto n_threads = 8;

	std::vector<pixel_map> results;
	for (int i = 0; i < n_threads; ++i) {
		pixel_map pixels(cam.image_height, std::vector<color>(cam.image_width));
		results.emplace_back(pixels);
	}

	auto render = [&cam, &results, &world](int thread_id) {
		cam.render(world, results[thread_id]);
	};

	std::vector<std::future<void>> handles;
	for (int i = 0; i < n_threads; ++i) {
		handles.emplace_back(std::async(std::launch::async, render, i));
	}

	for (int i = 0; i < n_threads; ++i) {
		handles[i].wait();
	}

	pixel_map final_image(
		cam.image_height, std::vector<color>(cam.image_width));
	for (int k = 0; k < results.size(); ++k) {
		auto pixels = results[k];
		for (int j = 0; j < pixels.size(); ++j) {
			for (int i = 0; i < pixels[0].size(); ++i) {
				final_image[j][i] += pixels[j][i];
			}
		}
	}

	std::cout << "P3\n"
			  << cam.image_width << ' ' << cam.image_height << "\n255\n";
	for (int j = 0; j < final_image.size(); ++j) {
		for (int i = 0; i < final_image[0].size(); ++i) {
			write_color(
				std::cout, final_image[j][i],
				cam.samples_per_pixel * n_threads);
		}
	}
}
