#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"
#include "utils/interval.h"

class hittable_list : public hittable {
   public:
	std::vector<std::shared_ptr<hittable>> objects;

	explicit hittable_list() noexcept = default;
	explicit hittable_list(const std::shared_ptr<hittable>& object) noexcept {
		add(object);
	}

	std::shared_ptr<hittable> deep_copy() const noexcept override {
		auto new_hittable_list = std::make_shared<hittable_list>(*this);
		new_hittable_list->clear();

		for (const auto& obj : objects) {
			new_hittable_list->add(obj->deep_copy());
		}

		return new_hittable_list;
	}

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