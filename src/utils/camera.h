#ifndef CAMERA_H
#define CAMERA_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "utils/color.h"
#include "utils/constants.h"
#include "utils/random.h"
#include "utils/vec3.h"

class camera {
   public:
	double aspect_ratio = 1.0;	// Ratio of image width over height
	int image_width = 100;		// Rendered image width in pixel count
	int samples_per_pixel = 10;
	int max_depth = 10;

	double vertical_fov = 90;
	point3 look_from = point3(0, 0, -1);
	point3 look_at = point3(0, 0, 0);
	vec3 vertical_up = vec3(0, 1, 0);

	void render(const hittable& world) {
		initialize();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		for (int j = 0; j < image_height; ++j) {
			std::clog << "\rScanlines remanaining: " << (image_height - j)
					  << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}

				write_color(std::cout, pixel_color, samples_per_pixel);
			}
		}

		std::clog << "\rDone.                   " << std::endl;
	}

   private:
	int image_height;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w;

	void initialize() {
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = look_from;

		const auto focal_length = (look_from - look_at).length();
		const auto theta = Constants::degress_to_radians(vertical_fov);
		auto h = tan(theta / 2.0);
		const auto viewport_height = 2.0 * h * focal_length;
		const auto viewport_width =
			viewport_height * (static_cast<double>(image_width) / image_height);

		w = unit_vector(look_from - look_at);
		u = unit_vector(cross(vertical_up, w));
		v = cross(w, u);

		const auto viewport_u = viewport_width * u;
		const auto viewport_v = viewport_height * -v;

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		const auto viewport_upper_left =
			center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc =
			viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	ray get_ray(int i, int j) {
		auto pixel_center =
			pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		auto pixel_sample = pixel_center + random_sample_in_square();

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 random_sample_in_square() const {
		auto px = -0.5 + random_double();
		auto py = -0.5 + random_double();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}

	color ray_color(const ray& r, int depth, const hittable& world) {
		if (depth <= 0) return color(0, 0, 0);

		hit_record rec;
		if (world.hit(r, interval(0.001, Constants::INF), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);

			return color(0, 0, 0);
		}

		return background_color(r);
	}

	color background_color(const ray& r) {
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif