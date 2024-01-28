#ifndef SCENES_H
#define SCENES_H

#include "objects/hittable_list.h"
#include "utils/camera.h"

namespace scenes::book_one {

void end(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void performance_analysis(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void simple_scene_rrt(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);

}  // namespace scenes::book_one

#endif