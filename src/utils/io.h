#ifndef IO_H
#define IO_H

#include "utils/pixel_map.h"

namespace io {

inline void display(
	const pixel_map& pixels, int height, int width, int samples_per_pixel) {
	std::cout << "P3\n" << width << ' ' << height << "\n255\n";

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			write_color(std::cout, pixels[j][i], samples_per_pixel);
		}
	}

	std::cout << std::flush;
}

}  // namespace io

#endif