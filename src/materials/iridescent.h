#ifndef IRIDESCENT_H
#define IRIDESCENT_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "utils/vec3.h"

class iridescent : public material {
   public:
	iridescent(bool _mate = false) : mate(_mate) {}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		if (mate) return mate_scatter(r_in, rec, attenuation, scattered);
		return shiny_scatter(r_in, rec, attenuation, scattered);
	}

   private:
	bool mate;

	bool mate_scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const {
		auto scatter_direction = rec.normal + random_unit_vector();
		if (scatter_direction.near_zero()) scatter_direction = rec.normal;
		scattered = ray(rec.p, scatter_direction);
		attenuation = 0.5 * (rec.normal + color(1, 1, 1));

		return true;
	}

	bool shiny_scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = 0.5 * (rec.normal + color(1, 1, 1));

		return dot(scattered.direction(), rec.normal) > 0;
	}
};

#endif