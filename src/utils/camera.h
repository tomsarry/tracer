#ifndef CAMERA_H
#define CAMERA_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "utils/color.h"
#include "utils/constants.h"
#include "utils/pixel_map.h"
#include "utils/random.h"
#include "utils/vec3.h"

class camera : public copyable<camera> {
   public:
	double aspect_ratio = 1.0;	// Ratio of image width over height
	int image_width = 100;		// Rendered image width in pixel count
	int image_height;
	int samples_per_pixel = 10;
	int max_depth = 10;
	color background = color(.7, .8, 1);

	double vertical_fov = 90;
	point3 look_from = point3(0, 0, -1);
	point3 look_at = point3(0, 0, 0);
	vec3 vertical_up = vec3(0, 1, 0);

	double defocus_angle = 0;
	double focus_dist = 10;

	std::shared_ptr<camera> deep_copy() const noexcept {
		return std::make_shared<camera>(*this);
	}

	void zoom(double n) { vertical_fov -= n; }

	void move_left() {
		look_from += vec3(-0.5, 0, 0);
		look_at += vec3(-0.5, 0, 0);
	}

	void move_right() {
		look_from += vec3(0.5, 0, 0);
		look_at += vec3(0.5, 0, 0);
	}

	void move_front() {
		look_from += vec3(0, 0, -0.5);
		look_at += vec3(0, 0, -0.5);
	}
	void move_back() {
		look_from += vec3(0, 0, 0.5);
		look_at += vec3(0, 0, 0.5);
	}

	void move_up() {
		look_from += vec3(0, 0.5, 0);
		look_at += vec3(0, 0.5, 0);
	}

	void move_down() {
		look_from += vec3(0, -0.5, 0);
		look_at += vec3(0, -0.5, 0);
	}

	void look_up() { look_at += vec3(0, 0.5, 0); }
	void look_down() { look_at += vec3(0, -0.5, 0); }

	void look_left() { look_at += vec3(-0.5, 0, 0); }
	void look_right() { look_at += vec3(0.5, 0, 0); }

	void render(std::shared_ptr<hittable> world, pixel_map& pixels) {
		initialize();

		for (int j = 0; j < image_height; ++j) {
			auto remaining = image_height - j;
			if (remaining % 10 == 0)
				std::clog << "\rScanlines remaining: " << (image_height - j)
						  << ' ' << std::flush;

			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}

				pixels[j][i] += pixel_color;
			}
		}

		std::clog << "\rDone.                         " << std::endl;
	}

   private:
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w;
	vec3 defocus_disk_u;
	vec3 defocus_disk_v;

	void initialize() {
		center = look_from;

		const auto theta = Constants::degress_to_radians(vertical_fov);
		auto h = tan(theta / 2.0);
		const auto viewport_height = 2.0 * h * focus_dist;
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
			center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc =
			viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		auto defocus_radius =
			focus_dist * tan(Constants::degress_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	ray get_ray(int i, int j) {
		auto pixel_center =
			pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		auto pixel_sample = pixel_center + random_sample_in_square();

		auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;
		auto ray_time = random_double();

		return ray(ray_origin, ray_direction, ray_time);
	}

	point3 defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	vec3 random_sample_in_square() const {
		auto px = -0.5 + random_double();
		auto py = -0.5 + random_double();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}

	color ray_color(
		const ray& r, int depth, const std::shared_ptr<hittable>& world) {
		if (depth <= 0) return color(0, 0, 0);

		hit_record rec;
		if (!world->hit(r, interval(0.001, Constants::INF), rec))
			return background;

		ray scattered;
		color attenuation;
		color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);
		if (!rec.mat->scatter(r, rec, attenuation, scattered))
			return color_from_emission;

		color color_from_scatter =
			attenuation * ray_color(scattered, depth - 1, world);

		return color_from_emission + color_from_scatter;
	}

	color background_color(const ray& r) {
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif