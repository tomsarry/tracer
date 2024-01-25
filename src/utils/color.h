#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "utils/interval.h"
#include "vec3.h"

using color = vec3;

inline void write_color(
	std::ostream& out, color pixel_color, int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	auto scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	const interval intensity(0.000, 0.999);
	out << static_cast<int>(256 * intensity.clamp(r)) << ' '
		<< static_cast<int>(256 * intensity.clamp(g)) << ' '
		<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif