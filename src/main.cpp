#include <cassert>
#include <iostream>

#include "objects/hittable.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "utils/color.h"
#include "utils/constants.h"
#include "utils/interval.h"
#include "utils/ray.h"
#include "utils/vec3.h"

int getImageHeight(int width, double aspect_ratio) {
	const auto result = static_cast<int>(width / aspect_ratio);
	assert(result >= 1);

	return result;
}

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, interval(0, Constants::INF), rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = getImageHeight(image_width, aspect_ratio);

	hittable_list world;

	world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(point3(0, -1, -1), 0.8));
	world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

	const auto focal_length = 1.0;
	const auto viewport_height = 2.0;
	const auto viewport_width =
		viewport_height * (static_cast<double>(image_width) / image_height);
	const auto camera_center = point3(0, 0, 0);

	const auto viewport_u = vec3(viewport_width, 0, 0);
	const auto viewport_v = vec3(0, -viewport_height, 0);

	const auto pixel_delta_u = viewport_u / image_width;
	const auto pixel_delta_v = viewport_v / image_height;

	const auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
									 viewport_u / 2 - viewport_v / 2;
	const auto pixel00_loc =
		viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = 0; j < image_height; ++j) {
		std::clog << "\rScanlines remanaining: " << (image_height - j) << ' '
				  << std::flush;
		for (int i = 0; i < image_width; ++i) {
			const auto pixel_center =
				pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			const auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.                   " << std::endl;
}