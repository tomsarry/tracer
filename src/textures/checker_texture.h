#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include <memory>

#include "texture.h"

class checker_texture : public texture {
   public:
	checker_texture(
		double _scale, std::shared_ptr<texture> _even,
		std::shared_ptr<texture> _odd)
		: inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

	checker_texture(double _scale, color c1, color c2)
		: inv_scale(1.0 / _scale),
		  even(std::make_shared<solid_color>(c1)),
		  odd(std::make_shared<solid_color>(c2)) {}

	color value(double u, double v, const point3& p) const override {
		auto x = static_cast<int>(std::floor(inv_scale * p.x()));
		auto y = static_cast<int>(std::floor(inv_scale * p.y()));
		auto z = static_cast<int>(std::floor(inv_scale * p.z()));

		bool isEven = (x + y + z) % 2 == 0;

		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}

   private:
	double inv_scale;
	std::shared_ptr<texture> even;
	std::shared_ptr<texture> odd;
};

#endif