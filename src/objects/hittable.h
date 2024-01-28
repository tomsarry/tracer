#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>

#include "utils/copyable.h"
#include "utils/interval.h"
#include "utils/ray.h"

class material;

class hit_record {
   public:
	point3 p;
	vec3 normal;
	std::shared_ptr<material> mat;
	double t;
	bool front_face;

	void set_normal_face(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable : public copyable<hittable> {
   public:
	virtual ~hittable() = default;

	virtual std::shared_ptr<hittable> deep_copy() const noexcept = 0;
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif