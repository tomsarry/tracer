#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include <vector>

#include "textures/image_asset.h"
#include "textures/texture.h"
#include "utils/color.h"
#include "utils/interval.h"
#include "utils/point2.h"

struct placement_info {
	int scaleX;
	int scaleY;
	point2 start;
};

class image_texture : public texture {
   public:
	image_texture(
		const char* filename, color bg, color fg,
		std::vector<placement_info> _placements)
		: image(filename),
		  background_color(bg),
		  scratch_color(fg),
		  placements(_placements) {}

	image_texture(const char* filename) : image(filename) {}

	color value(double u, double v, const point3& p) const override {
		if (image.height() <= 0) return color(0, 1, 1);

		auto lowest = 255;

		for (const auto& placement : placements) {
			auto start = placement.start;
			auto scaleX = placement.scaleX;
			auto scaleY = placement.scaleY;

			// if is_outside return bg color
			if (is_outside_of_image(u, v, placement)) continue;

			auto new_u = u - start.x;
			auto new_v = v - start.y;

			new_u = interval(0, 1).clamp(new_u);
			new_v = 1.0 - interval(0, 1).clamp(new_v);

			auto i = static_cast<int>(scaleX * new_u * image.width()) %
					 image.width();
			auto j = static_cast<int>(scaleY * new_v * image.height()) %
					 image.height();
			auto pixel = image.pixel_data(i, j);

			auto new_intensity = pixel[0];
			if (new_intensity < lowest) lowest = new_intensity;

			// only choose the color that is closest to a scratch if overlap
			// between images
		}

		auto color_scale = 1.0 / 255.0;
		auto best_color = color(
			color_scale * lowest, color_scale * lowest, color_scale * lowest);

		return lerp(best_color);
	}

   private:
	image_asset image;
	color background_color;
	color scratch_color;
	std::vector<placement_info> placements;

	// gets the darkest color
	color get_darkest(const color& c1, const color& c2) const {
		if (c1.x() + c1.y() + c1.z() > c2.x() + c2.y() + c2.z()) return c2;
		return c1;
	}

	bool is_outside_of_image(
		double u, double v, const placement_info& placement) const {
		return u < placement.start.x || v < placement.start.y ||
			   u > placement.start.x + 1.0 / placement.scaleX ||
			   v > placement.start.y + 1.0 / placement.scaleY;
	}

	color lerp(const color& pixel_color) const {
		return (background_color - scratch_color) * pixel_color + scratch_color;
	}
};

#endif