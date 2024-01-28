#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include "objects/hittable_list.h"
#include "utils/camera.h"
#include "utils/pixel_map.h"

enum ThreadNumber {
	ONE = 1,
	FOUR = 4,
	EIGHT = 8,
};

pixel_map multi_thread_render(
	ThreadNumber t, camera& cam, const hittable_list& world);

#endif