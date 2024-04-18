#ifndef METAL_H
#define METAL_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "utils/vec3.h"

class metal : public material {
   public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	std::shared_ptr<material> deep_copy() const noexcept override {
		return std::make_shared<metal>(*this);
	}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered =
			ray(rec.p, reflected + fuzz * random_unit_vector(), r_in.time());
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}

   private:
	color albedo;
	double fuzz;
};

#endif