#ifndef BVH_H
#define BVH_H

#include <algorithm>
#include <memory>

#include "objects/aabb.h"
#include "objects/hittable.h"
#include "objects/hittable_list.h"

class bvh_node : public hittable {
   public:
	bvh_node(const hittable_list& list)
		: bvh_node(list.objects, 0, list.objects.size()) {}

	bvh_node(
		const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start,
		size_t end) {
		auto objects = src_objects;

		int axis = random_int(0, 2);
		auto comparator = (axis == 0)	? box_x_compare
						  : (axis == 1) ? box_y_compare
										: box_z_compare;

		size_t object_span = end - start;

		if (object_span == 1) {
			left = right = objects[start];
		} else if (object_span == 2) {
			if (comparator(objects[start], objects[start + 1])) {
				left = objects[start];
				right = objects[start + 1];
			} else {
				left = objects[start + 1];
				right = objects[start];
			}
		} else {
			std::sort(
				objects.begin() + start, objects.begin() + end, comparator);

			auto mid = start + object_span / 2;
			left = std::make_shared<bvh_node>(objects, start, mid);
			right = std::make_shared<bvh_node>(objects, mid, end);
		}

		bbox = aabb(left->bounding_box(), right->bounding_box());
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		if (!bbox.hit(r, ray_t)) return false;

		bool hit_left = left->hit(r, ray_t, rec);
		bool hit_right = right->hit(
			r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

		return hit_left || hit_right;
	}

	std::shared_ptr<hittable> deep_copy() const noexcept override {
		auto new_bvh = std::make_shared<bvh_node>(*this);

		new_bvh->left = left->deep_copy();
		new_bvh->right = right->deep_copy();

		return new_bvh;
	}

	aabb bounding_box() const override { return bbox; }

   private:
	std::shared_ptr<hittable> left;
	std::shared_ptr<hittable> right;
	aabb bbox;

	static bool box_compare(
		const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b,
		int axis_index) {
		return a->bounding_box().axis(axis_index).min <
			   b->bounding_box().axis(axis_index).min;
	}

	static bool box_x_compare(
		const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
		return box_compare(a, b, 0);
	}
	static bool box_y_compare(
		const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
		return box_compare(a, b, 1);
	}
	static bool box_z_compare(
		const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
		return box_compare(a, b, 2);
	}
};

#endif