#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>

namespace Constants {

constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

constexpr double degress_to_radians(double degrees) {
	return degrees * PI / 180.0;
}

}  // namespace Constants

#endif
