#ifndef POINT2_H
#define POINT2_H

#include <iostream>

struct point2 {
	double x;
	double y;

	bool operator==(const point2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const point2& other) { return !(*this == other); }
};

inline std::ostream& operator<<(std::ostream& os, const point2& point) {
	return os << '{' << point.x << ',' << point.y << '}' << std::endl;
}

#endif