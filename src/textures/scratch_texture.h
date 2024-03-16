#ifndef SCRATCH_TEXTURE_H
#define SCRATCH_TEXTURE_H

#include "textures/texture.h"
#include "utils/point2.h"

class scratch_texture : public texture {
   public:
	// todo: enable list of scratch coordinates
	// todo: enable control points for curvature
	scratch_texture(point2 _p0, point2 _p1, double _epsilon)
		: p0(_p0), p1(_p1), epsilon(_epsilon) {}

	color value(double u, double v, const point3& p) const override {
		if (is_scratch(u, v)) return {1, 0, 1};
		return {1, 1, 1};
	}

   private:
	point2 p0;
	point2 p1;
	double epsilon;

	bool is_outside(
		double v, double u, double x0, double y0, double x1, double y1) const {
		const auto max_x = fmax(x0, x1);
		const auto max_y = fmax(y0, y1);
		const auto min_x = fmin(x0, x1);
		const auto min_y = fmin(y0, y1);

		return v > max_y || v < min_y || u < min_x || u > max_x;
	}

	bool is_scratch(double u, double v) const {
		const auto x0 = p0.x;
		const auto y0 = p0.y;
		const auto x1 = p1.x;
		const auto y1 = p1.y;

		const auto slope = (y1 - y0) / (x1 - x0);
		const auto intercept = y0 - slope * x0;

		if (is_outside(v, u, x0, y0, x1, y1)) return false;

		// is (x,y) on the line?
		if (fabs(slope * u + intercept - v) < epsilon) return true;
		return false;
	}
};

#endif
