#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

// Returns a random real between [0, 1)
inline double random_double() { return rand() / (RAND_MAX + 1.0); }

// Returns a random real between [min,max)
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

#endif