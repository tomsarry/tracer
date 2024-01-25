#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"
#include "utils/interval.h"

class hittable_list : public hittable {
   public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(std::shared_ptr<hittable> object) { objects.emplace_back(object); }

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		hit_record temp_rec;
		bool has_hit = false;
		auto closest_so_far = ray_t.max;

		for (const auto& object : objects) {
			if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
				has_hit = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return has_hit;
	}
};

#endif