#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "utils/color.h"
#include "utils/vec3.h"

class lambertian : public material {
   public:
	lambertian(const color& a) : albedo(a) {}

	std::shared_ptr<material> deep_copy() const noexcept override {
		return std::make_shared<lambertian>(*this);
	}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero()) scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

   private:
	color albedo;
};

#endif