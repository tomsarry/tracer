#ifndef IO_H
#define IO_H

#include <fstream>

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

inline void display_occupancy_map(const std::vector<std::vector<bool>>& map) {
	const color white{1, 1, 1};
	const color black{0, 0, 0};

	const auto height = map.size();
	const auto width = map.size();
	std::cout << "P3\n" << width << ' ' << height << "\n255\n";

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			auto color = black;
			if (map[j][i]) color = white;

			write_color(std::cout, color, 1);
		}
	}

	std::cout << std::flush;
}

inline void write_to_file(
	const char* file_name, const pixel_map& pixels, int height, int width,
	int samples_per_pixel) {
	std::ofstream output_file;
	output_file.open(file_name);

	output_file << "P3\n" << width << ' ' << height << "\n255\n";

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			write_color(output_file, pixels[j][i], samples_per_pixel);
		}
	}

	output_file.close();
}
}  // namespace io

#endif