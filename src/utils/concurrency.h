#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include <future>

#include "objects/hittable_list.h"
#include "utils/camera.h"
#include "utils/pixel_map.h"

inline pixel_map multi_thread_render(
	int N_THREADS, camera& cam, const hittable_list& world) {
	std::vector<pixel_map> results;
	for (int i = 0; i < N_THREADS; ++i) {
		pixel_map pixels(cam.image_height, std::vector<color>(cam.image_width));
		results.emplace_back(pixels);
	}

	auto render = [&cam, &results, &world](int thread_id) {
		cam.render(world, results[thread_id]);
	};

	std::vector<std::future<void>> handles;
	for (int i = 0; i < N_THREADS; ++i) {
		handles.emplace_back(std::async(std::launch::async, render, i));
	}

	for (int i = 0; i < N_THREADS; ++i) {
		handles[i].wait();
	}

	pixel_map final_image(
		cam.image_height, std::vector<color>(cam.image_width));
	for (int k = 0; k < results.size(); ++k) {
		auto pixels = results[k];
		for (int j = 0; j < cam.image_height; ++j) {
			for (int i = 0; i < cam.image_width; ++i) {
				final_image[j][i] += pixels[j][i];
			}
		}
	}

	return final_image;
}

#endif