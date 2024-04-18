#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H

#include <memory>

#include "materials/material.h"
#include "textures/texture.h"

class diffuse_light : public material {
   public:
	diffuse_light(std::shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(std::make_shared<solid_color>(c)) {}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) override {
		return false;
	}

	color emitted(double u, double v, const point3& p) const override {
		return emit->value(u, v, p);
	}

	std::shared_ptr<material> deep_copy() const noexcept override {
		return std::make_shared<diffuse_light>(*this);
	}

   private:
	std::shared_ptr<texture> emit;
};

#endif