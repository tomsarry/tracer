#ifndef SCRATCHED_METAL_H
#define SCRATCHED_METAL_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "textures/image_texture.h"
#include "utils/vec3.h"

class scratched_metal : public material {
   public:
	scratched_metal(
		const color& a, double f, const char* filename, placement_info& i,
		bool r)
		: albedo(a),
		  fuzz(f < 1 ? f : 1),
		  image(filename),
		  placement(i),
		  repeat(r) {}

	std::shared_ptr<material> deep_copy() const noexcept override {
		return std::make_shared<scratched_metal>(*this);
	}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		vec3 normal = rec.normal;

		if (shouldPerturbNormal(rec)) {
			normal = unit_vector(perturbNormal(rec));
		}

		vec3 reflected = reflect(unit_vector(r_in.direction()), normal);
		scattered =
			ray(rec.p, reflected + fuzz * random_unit_vector(), r_in.time());
		attenuation = albedo;
		return dot(scattered.direction(), normal) > 0;
	}

   private:
	color albedo;
	double fuzz;
	image_asset image;
	placement_info placement;
	bool repeat;

	vec3 perturbNormal(const hit_record& rec) const {
		auto u = interval(0, 1).clamp(rec.u - placement.start.x);
		auto v = interval(0, 1).clamp(rec.v - placement.start.y);

		auto i = static_cast<int>(placement.scaleX * u * image.width()) %
				 image.width();
		auto j = static_cast<int>(placement.scaleY * v * image.height()) %
				 image.height();
		auto pixel = image.pixel_data(i, j);

		vec3 color(pixel[0], pixel[1], pixel[2]);

		vec3 T(-1, 0, 0);
		vec3 B(0, 0, 1);
		vec3 N(0, 1, 0);

		auto normal = ((color / 256.) - vec3(0.5, 0.5, 0.5)) * 2;

		auto x = dot(T, normal);
		auto y = dot(B, normal);
		auto z = dot(N, normal);

		return vec3(x, y, z);
	}

	bool shouldPerturbNormal(const hit_record& rec) const {
		if (repeat) return true;
		if (!is_outside_of_image(rec.u, rec.v, placement)) return true;
		return false;
	}

	bool is_outside_of_image(
		double u, double v, const placement_info& placement) const {
		return u < placement.start.x || v < placement.start.y ||
			   u > placement.start.x + 1.0 / placement.scaleX ||
			   v > placement.start.y + 1.0 / placement.scaleY;
	}
};

#endif