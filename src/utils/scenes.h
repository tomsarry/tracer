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
void video(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);

}  // namespace scenes::book_one

namespace scenes::book_two {
void bouncing_spheres(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void two_spheres(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void earth(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void quads(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
}  // namespace scenes::book_two

namespace scenes::scratches {
void monkey(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void single(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void close(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void far(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
void complex(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);

// grazing angles
void complex2(
	hittable_list& world, camera& cam, int samples_per_pixel, int max_depth);
}  // namespace scenes::scratches

#endif