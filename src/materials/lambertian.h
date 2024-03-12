#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "textures/texture.h"
#include "utils/color.h"
#include "utils/vec3.h"

class lambertian : public material {
   public:
	lambertian(const color& a) : albedo(std::make_shared<solid_color>(a)) {}
	lambertian(std::shared_ptr<texture> a) : albedo(a) {}

	std::shared_ptr<material> deep_copy() const noexcept override {
		return std::make_shared<lambertian>(*this);
	}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero()) scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

   private:
	std::shared_ptr<texture> albedo;
};

#endif