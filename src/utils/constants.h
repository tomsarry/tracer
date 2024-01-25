#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

constexpr double degress_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

#endif
