#ifndef POINT2_H
#define POINT2_H

#include <iostream>

struct point2 {
	double x;
	double y;
};

inline std::ostream& operator<<(std::ostream& os, const point2& point) {
	return os << '{' << point.x << ',' << point.y << '}' << std::endl;
}

#endif