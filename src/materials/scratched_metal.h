#ifndef SCRATCHED_METAL_H
#define SCRATCHED_METAL_H

#include "materials/material.h"
#include "objects/hittable.h"
#include "textures/image_texture.h"
#include "utils/constants.h"
#include "utils/random.h"
#include "utils/vec3.h"

namespace {
color outward_normal(128, 128, 255);
}  // namespace

struct kernel {
	int top_left_i;
	int top_left_j;
	int bottom_right_i;
	int bottom_right_j;
};

class scratched_metal : public material {
   public:
	scratched_metal(
		const color& a, double f, const char* filename, placement_info& i,
		bool r)
		: albedo(a),
		  fuzz(f < 1 ? f : 1),
		  image(filename),
		  placement(i),
		  repeat(r) {}

	std::shared_ptr<material> deep_copy() const noexcept override {
		return std::make_shared<scratched_metal>(*this);
	}

	bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		vec3 normal = rec.normal;

		if (shouldPerturbNormal(rec)) {
			normal = unit_vector(perturbNormal(rec));
		}

		vec3 reflected = reflect(unit_vector(r_in.direction()), normal);
		scattered =
			ray(rec.p, reflected + fuzz * random_unit_vector(), r_in.time());
		attenuation = albedo;
		return dot(scattered.direction(), normal) > 0;
	}

   private:
	color albedo;
	double fuzz;
	image_asset image;
	placement_info placement;
	bool repeat;

	vec3 perturbNormal(const hit_record& rec) const {
		auto top_left_u =
			interval(0, 1).clamp(rec.top_left.x - placement.start.x);
		auto top_left_v =
			interval(0, 1).clamp(rec.top_left.y - placement.start.y);
		auto top_left_i =
			static_cast<int>(placement.scaleX * top_left_u * image.width()) %
			image.width();
		auto top_left_j =
			static_cast<int>(placement.scaleY * top_left_v * image.height()) %
			image.height();

		auto bottom_right_u =
			interval(0, 1).clamp(rec.bottom_right.x - placement.start.x);
		auto bottom_right_v =
			interval(0, 1).clamp(rec.bottom_right.y - placement.start.y);
		auto bottom_right_i =
			static_cast<int>(
				placement.scaleX * bottom_right_u * image.width()) %
			image.width();
		auto bottom_right_j =
			static_cast<int>(
				placement.scaleY * bottom_right_v * image.height()) %
			image.height();

		kernel k{top_left_i, top_left_j, bottom_right_i, bottom_right_j};

		auto u = interval(0, 1).clamp(rec.u - placement.start.x);
		auto v = interval(0, 1).clamp(rec.v - placement.start.y);

		auto i = static_cast<int>(placement.scaleX * u * image.width()) %
				 image.width();
		auto j = static_cast<int>(placement.scaleY * v * image.height()) %
				 image.height();
		auto pixel = image.pixel_data(i, j);

		vec3 color(pixel[0], pixel[1], pixel[2]);

		const auto radius{1};
		auto interesting = getClosestScratchTexels(i, j, radius, k);

		// if did not touch a scratch and there is a neighbouring scratch,
		// pick at random a new normal
		if (color == outward_normal && !interesting.empty()) {
			color = interesting[random_int(0, interesting.size() - 1)];
		}

		vec3 T(-1, 0, 0);
		vec3 B(0, 0, 1);
		vec3 N(0, 1, 0);

		auto normal = ((color / 256.) - vec3(0.5, 0.5, 0.5)) * 2;

		auto x = dot(T, normal);
		auto y = dot(B, normal);
		auto z = dot(N, normal);

		return vec3(x, y, z);
	}

	// get normals from "interesting" texels around a radius from initial texel
	std::vector<color> getCloseScratchTexels(int i, int j, int radius) const {
		std::vector<color> results;

		// enumerate coordinates of neighbours
		// square kernel, might overlap other neighbour pixels
		for (int ni = i - radius; ni < i + radius; ++ni) {
			for (int nj = j - radius; nj < j + radius; ++nj) {
				int i = ni % image.width();
				int j = nj % image.height();

				// store "interesting" normal
				auto texel = image.pixel_data(i, j);
				color c(texel[0], texel[1], texel[2]);
				if (c != outward_normal) results.emplace_back(c);
			}
		}

		return results;
	}

	std::vector<color> getClosestScratchTexels(
		int i, int j, int radius, const kernel& k) const {
		std::vector<color> results;
		auto minDistance = Constants::INF;

		auto kernel_size = (k.bottom_right_i - k.top_left_i) *
						   (k.top_left_j - k.bottom_right_j);
		// todo: think of this edge case
		if (kernel_size <= 0) return results;

		// enumerate coordinates of neighbours
		// square kernel, might overlap other neighbour pixels
		for (int ni = k.top_left_i; ni <= k.bottom_right_i; ++ni) {
			for (int nj = k.bottom_right_j; nj <= k.top_left_j; ++nj) {
				int i = ni % image.width();
				int j = nj % image.height();

				// store "interesting" normal
				auto texel = image.pixel_data(i, j);
				color c(texel[0], texel[1], texel[2]);

				// todo: replace with euclidean distance?
				auto manhattan_distance = abs(ni - i) + abs(nj - j);
				if (c != outward_normal) {
					if (minDistance == manhattan_distance)
						results.emplace_back(c);

					if (minDistance > manhattan_distance) {
						results = {c};
						minDistance = manhattan_distance;
					}
				}
			}
		}

		return results;
	}

	bool shouldPerturbNormal(const hit_record& rec) const {
		if (repeat) return true;
		if (!is_outside_of_image(rec.u, rec.v, placement)) return true;
		return false;
	}

	bool is_outside_of_image(
		double u, double v, const placement_info& placement) const {
		return u < placement.start.x || v < placement.start.y ||
			   u > placement.start.x + 1.0 / placement.scaleX ||
			   v > placement.start.y + 1.0 / placement.scaleY;
	}
};

#endif