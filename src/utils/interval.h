#ifndef INTERVAL_H
#define INTERVAL_H

#include "utils/constants.h"

class interval {
   public:
	double min;
	double max;

	interval() : min(+Constants::INF), max(-Constants::INF) {}
	interval(double _min, double _max) : min(_min), max(_max) {}

	bool contains(double x) const { return x >= min && x <= max; }

	bool surrounds(double x) const { return x > min && x < max; }

	static const interval EMPTY, UNIVERSE;
};

const static interval EMPTY(+Constants::INF, -Constants::INF);
const static interval UNIVERSE(-Constants::INF, +Constants::INF);

#endif