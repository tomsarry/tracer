#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils/color.h"
#include "utils/ray.h"

class hit_record;

class material {
   public:
	virtual ~material() = default;

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const = 0;
};

#endif