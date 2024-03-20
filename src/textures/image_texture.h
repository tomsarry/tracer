#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "textures/image_asset.h"
#include "textures/texture.h"
#include "utils/color.h"
#include "utils/interval.h"
#include "utils/point2.h"

class image_texture : public texture {
   public:
	image_texture(
		const char* filename, color bg, color fg, int _scaleX, int _scaleY,
		point2 _start)
		: image(filename),
		  background_color(bg),
		  scratch_color(fg),
		  scaleX(_scaleX),
		  scaleY(_scaleY),
		  start(_start) {}

	image_texture(const char* filename) : image(filename) {}

	color value(double u, double v, const point3& p) const override {
		if (image.height() <= 0) return color(0, 1, 1);

		// if is_outside return bg color
		if (u < start.x || v < start.y) return background_color;
		if (u > start.x + 1.0 / scaleX || v > start.y + 1.0 / scaleY)
			return background_color;

		u -= start.x;
		v -= start.y;

		u = interval(0, 1).clamp(u);
		v = 1.0 - interval(0, 1).clamp(v);

		auto i = static_cast<int>(scaleX * u * image.width()) % image.width();
		auto j = static_cast<int>(scaleY * v * image.height()) % image.height();
		auto pixel = image.pixel_data(i, j);

		auto color_scale = 1.0 / 255.0;
		auto pixel_color = color(
			color_scale * pixel[0], color_scale * pixel[1],
			color_scale * pixel[2]);

		return lerp(pixel_color);
	}

   private:
	image_asset image;
	color background_color;
	color scratch_color;
	int scaleX, scaleY;
	point2 start;

	color lerp(const color& pixel_color) const {
		return (background_color - scratch_color) * pixel_color + scratch_color;
	}
};

#endif