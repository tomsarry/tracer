#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "textures/image_asset.h"
#include "textures/texture.h"
#include "utils/interval.h"

class image_texture : public texture {
   public:
	image_texture(const char* filename) : image(filename) {}

	color value(double u, double v, const point3& p) const override {
		if (image.height() <= 0) return color(0, 1, 1);

		u = interval(0, 1).clamp(u);
		v = 1.0 - interval(0, 1).clamp(v);

		auto i = static_cast<int>(u * image.width());
		auto j = static_cast<int>(v * image.height());
		auto pixel = image.pixel_data(i, j);

		auto color_scale = 1.0 / 255.0;
		return color(
			color_scale * pixel[0], color_scale * pixel[1],
			color_scale * pixel[2]);
	}

   private:
	image_asset image;
};

#endif