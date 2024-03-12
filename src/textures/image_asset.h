#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

class image_asset {
   public:
	image_asset() : data(nullptr) {}

	image_asset(const char* image_filename) {
		auto filename = std::string(image_filename);
		std::string imagedir = "assets";

		auto filepath = imagedir + '/' + filename;
		if (load(filepath)) return;

		std::cerr << "ERROR: Could not load image file '" << filepath << ".\n";
	}

	~image_asset() { STBI_FREE(data); }

	bool load(const std::string filename) {
		auto n = bytes_per_pixel;
		data = stbi_load(
			filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
		bytes_per_scanline = image_width * bytes_per_pixel;
		return data != nullptr;
	}

	int width() const { return (data == nullptr) ? 0 : image_width; }
	int height() const { return (data == nullptr) ? 0 : image_height; }

	const unsigned char* pixel_data(int x, int y) const {
		static unsigned char magenta[] = {255, 0, 255};
		if (data == nullptr) return magenta;

		x = clamp(x, 0, image_width);
		y = clamp(y, 0, image_height);

		return data + y * bytes_per_scanline + x * bytes_per_pixel;
	}

   private:
	const int bytes_per_pixel = 3;
	unsigned char* data;
	int image_width, image_height;
	int bytes_per_scanline;

	static int clamp(int x, int low, int high) {
		if (x < low) return low;
		if (x < high) return x;
		return high - 1;
	}
};

#endif