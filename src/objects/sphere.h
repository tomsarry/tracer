#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "utils/interval.h"
#include "utils/vec3.h"

class sphere : public hittable {
   public:
	sphere(point3 c, double r) : center(c), radius(r) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec)
		const override {
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		auto sqrtd = sqrt(discriminant);

		auto root = (-half_b - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (-half_b + sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_normal_face(r, outward_normal);

		return true;
	}

   private:
	point3 center;
	double radius;
};

#endif