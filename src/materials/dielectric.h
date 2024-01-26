#ifndef DIALETRIC_H
#define DIALETRIC_H

#include "materials/material.h"
#include "objects/hittable.h"

class dielectric : public material {
   public:
	dielectric(double refraction_idx) : refraction_index(refraction_idx) {}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio =
			rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.direction());
		vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

		auto cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		auto sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		auto cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract) {
			direction = reflect(unit_direction, rec.normal);
		} else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = ray(rec.p, refracted);
		return true;
	}

   private:
	double refraction_index;
};

#endif