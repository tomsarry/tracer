#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include "utils/color.h"
#include "utils/ray.h"

class hit_record;

class material {
   public:
	virtual ~material() = default;

	virtual std::shared_ptr<material> deep_copy() const noexcept = 0;

	virtual color emitted(double u, double v, const point3& p) const {
		return color(0, 0, 0);
	}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) = 0;
};

#endif