#ifndef SCRATCH_TEXTURE_H
#define SCRATCH_TEXTURE_H

#include <vector>

#include "textures/scratch.h"
#include "textures/texture.h"
#include "utils/interval.h"
#include "utils/io.h"
#include "utils/point2.h"

namespace {
const auto N = 100;
}  // namespace

class scratch_texture : public texture {
   public:
	// todo: efficient structure for intersection
	// todo: enable control points for curvature
	// todo: add color scratch and background
	scratch_texture(std::vector<scratch>& _scratches, color bg, color scratch)
		: scratches(_scratches), background_color(bg), scratch_color(scratch) {
		setup();
	}

	color value(double u, double v, const point3& p) const override {
		if (is_scratch(u, v)) return scratch_color;
		return background_color;
	}

   private:
	std::vector<scratch> scratches;
	color background_color;
	color scratch_color;
	// divide texture in tiles and precompute if scratch is present
	// keep a list of scratches intersecting each tile
	std::vector<std::vector<bool>> occupancy_tiles;

	void setup() {
		std::vector<std::vector<bool>> tiles(N, std::vector<bool>(N));
		occupancy_tiles = tiles;

		for (const auto& scratch : scratches) {
			const auto epsilon = .00001;
			const auto du = scratch.end.x - scratch.start.x;
			const auto dv = scratch.end.y - scratch.start.y;
			auto p = scratch.start;

			// while p in bbox of scratch
			while (!is_outside(
				p.y, p.x, scratch.start.x, scratch.start.y, scratch.end.x,
				scratch.end.y)) {
				// 	locate p on grid -> tile
				auto index_x = static_cast<int>(p.x * N);
				auto index_y = static_cast<int>(p.y * N);

				interval(0, N - 1).clamp(index_x);
				interval(0, N - 1).clamp(index_y);

				//  mark tile as occupied
				occupancy_tiles[index_y][index_x] = true;

				p.x += du * epsilon;
				p.y += dv * epsilon;
			}
		}

		// io::display_occupancy_map(tiles);
	}

	bool is_outside(
		double v, double u, double x0, double y0, double x1, double y1) const {
		const auto max_x = fmax(x0, x1);
		const auto max_y = fmax(y0, y1);
		const auto min_x = fmin(x0, x1);
		const auto min_y = fmin(y0, y1);

		return v > max_y || v < min_y || u < min_x || u > max_x;
	}

	bool is_scratch(double u, double v) const {
		auto index_x = static_cast<int>(u * N);
		auto index_y = static_cast<int>(v * N);

		interval(0, N - 1).clamp(index_x);
		interval(0, N - 1).clamp(index_y);
		if (!occupancy_tiles[index_y][index_x]) return false;

		for (const auto& s : scratches) {
			const auto x0 = s.start.x;
			const auto y0 = s.start.y;
			const auto x1 = s.end.x;
			const auto y1 = s.end.y;

			const auto slope = (y1 - y0) / (x1 - x0);
			const auto intercept = y0 - slope * x0;

			if (is_outside(v, u, x0, y0, x1, y1)) continue;

			// is (x,y) on the line?
			if (fabs(slope * u + intercept - v) < s.epsilon) return true;
		}
		return false;
	}
};

#endif
