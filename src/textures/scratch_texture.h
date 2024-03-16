#ifndef SCRATCH_TEXTURE_H
#define SCRATCH_TEXTURE_H

#include "textures/texture.h"

class scratch_texture : public texture {
   public:
	// todo: refactor to objects
	// todo: enable list of scratch coordinates
	// todo: enable control points for curvature
	scratch_texture(
		double _x0, double _y0, double _x1, double _y1, double _epsilon)
		: x0(_x0), y0(_y0), x1(_x1), y1(_y1), epsilon(_epsilon) {}

	// P0 = (0.1, 0.1)
	// P1 = (0.5, 0.2)
	color value(double u, double v, const point3& p) const override {
		if (is_scratch(u, v)) return {1, 0, 1};
		return {1, 1, 1};
	}

   private:
	double x0;
	double y0;
	double x1;
	double y1;
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
		const auto slope = (y1 - y0) / (x1 - x0);
		const auto intercept = y0 - slope * x0;

		if (is_outside(v, u, x0, y0, x1, y1)) return false;

		// is (x,y) on the line?
		if (fabs(slope * u + intercept - v) < epsilon) return true;
		return false;
	}
};

#endif
