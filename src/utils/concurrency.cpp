#include "concurrency.h"

#include <future>

#include "utils/color.h"
#include "utils/pixel_map.h"

void add_pixel_maps(
	pixel_map& dest, const pixel_map& source, int width, int height) {
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			dest[j][i] += source[j][i];
		}
	}
}

pixel_map render_using_one_thread(camera& cam, const hittable_list& world) {
	std::cerr << "render on 1 thread with " << cam.samples_per_pixel
			  << std::endl;

	pixel_map pixels(cam.image_height, std::vector<color>(cam.image_width));

	cam.render(std::make_shared<hittable_list>(world), pixels);

	return pixels;
}

pixel_map render_using_four_threads(camera& cam, const hittable_list& world) {
	if (cam.samples_per_pixel % 4 != 0)
		std::cerr << "Warning: not exact division of labour: "
				  << cam.samples_per_pixel << "and 4 cores." << std::endl;

	cam.samples_per_pixel /= 4;

	std::cerr << "render on 4 threads with " << cam.samples_per_pixel
			  << std::endl;

	const auto height = cam.image_height;
	const auto width = cam.image_width;

	auto world1 = std::make_shared<hittable_list>(world);
	auto world2 = world.deep_copy();
	auto world3 = world.deep_copy();
	auto world4 = world.deep_copy();

	auto cam2 = cam.deep_copy();
	auto cam3 = cam.deep_copy();
	auto cam4 = cam.deep_copy();

	pixel_map pixels1(height, std::vector<color>(width));
	auto render1 = [&]() { cam.render(world1, pixels1); };

	pixel_map pixels2(height, std::vector<color>(width));
	auto render2 = [&]() { cam2->render(world2, pixels2); };

	pixel_map pixels3(height, std::vector<color>(width));
	auto render3 = [&]() { cam3->render(world3, pixels3); };

	pixel_map pixels4(height, std::vector<color>(width));
	auto render4 = [&]() { cam4->render(world4, pixels4); };

	auto handle1 = std::async(std::launch::async, render1);
	auto handle2 = std::async(std::launch::async, render2);
	auto handle3 = std::async(std::launch::async, render3);
	auto handle4 = std::async(std::launch::async, render4);

	handle1.wait();
	handle2.wait();
	handle3.wait();
	handle4.wait();

	add_pixel_maps(pixels1, pixels2, width, height);
	add_pixel_maps(pixels1, pixels3, width, height);
	add_pixel_maps(pixels1, pixels4, width, height);

	return pixels1;
}

pixel_map render_using_eight_threads(camera& cam, const hittable_list& world) {
	if (cam.samples_per_pixel % 8 != 0)
		std::cerr << "Warning: not exact division of labour: "
				  << cam.samples_per_pixel << "and 8 cores." << std::endl;

	cam.samples_per_pixel /= 8;

	std::cerr << "render on 8 threads with " << cam.samples_per_pixel
			  << std::endl;

	const auto height = cam.image_height;
	const auto width = cam.image_width;

	pixel_map pixels1(height, std::vector<color>(width));
	// auto render1 = [&]() { cam.render(world, pixels1); };

	// pixel_map pixels2(height, std::vector<color>(width));
	// auto render2 = [&]() { cam.render(world, pixels2); };

	// pixel_map pixels3(height, std::vector<color>(width));
	// auto render3 = [&]() { cam.render(world, pixels3); };

	// pixel_map pixels4(height, std::vector<color>(width));
	// auto render4 = [&]() { cam.render(world, pixels4); };

	// pixel_map pixels5(height, std::vector<color>(width));
	// auto render5 = [&]() { cam.render(world, pixels5); };

	// pixel_map pixels6(height, std::vector<color>(width));
	// auto render6 = [&]() { cam.render(world, pixels6); };

	// pixel_map pixels7(height, std::vector<color>(width));
	// auto render7 = [&]() { cam.render(world, pixels7); };

	// pixel_map pixels8(height, std::vector<color>(width));
	// auto render8 = [&]() { cam.render(world, pixels8); };

	// auto handle1 = std::async(std::launch::async, render1);
	// auto handle2 = std::async(std::launch::async, render2);
	// auto handle3 = std::async(std::launch::async, render3);
	// auto handle4 = std::async(std::launch::async, render4);
	// auto handle5 = std::async(std::launch::async, render4);
	// auto handle6 = std::async(std::launch::async, render4);
	// auto handle7 = std::async(std::launch::async, render4);
	// auto handle8 = std::async(std::launch::async, render4);

	// handle1.wait();
	// handle2.wait();
	// handle3.wait();
	// handle4.wait();
	// handle5.wait();
	// handle6.wait();
	// handle7.wait();
	// handle8.wait();

	// add_pixel_maps(pixels1, pixels2, width, height);
	// add_pixel_maps(pixels1, pixels3, width, height);
	// add_pixel_maps(pixels1, pixels4, width, height);
	// add_pixel_maps(pixels1, pixels5, width, height);
	// add_pixel_maps(pixels1, pixels6, width, height);
	// add_pixel_maps(pixels1, pixels7, width, height);
	// add_pixel_maps(pixels1, pixels8, width, height);

	return pixels1;
}

pixel_map multi_thread_render(
	ThreadNumber t, camera& cam, const hittable_list& world) {
	switch (t) {
		case ThreadNumber::ONE:
			return render_using_one_thread(cam, world);
		case ThreadNumber::FOUR:
			return render_using_four_threads(cam, world);
		case ThreadNumber::EIGHT:
			return render_using_eight_threads(cam, world);
	}

	assert(false);
}

// broken lol
// pixel_map multi_thread_render(
// 	int N_THREADS, camera& cam, const hittable_list& world) {
// 	std::vector<pixel_map> results;
// 	for (int i = 0; i < N_THREADS; ++i) {
// 		pixel_map pixels(cam.image_height, std::vector<color>(cam.image_width));
// 		results.emplace_back(pixels);
// 	}

// 	auto render = [&cam, &results, &world](int thread_id) {
// 		cam.render(world, results[thread_id]);
// 	};

// 	std::vector<std::future<void>> handles;
// 	for (int i = 0; i < N_THREADS; ++i) {
// 		handles.emplace_back(std::async(std::launch::async, render, i));
// 	}

// 	for (int i = 0; i < N_THREADS; ++i) {
// 		handles[i].wait();
// 	}

// 	pixel_map final_image(
// 		cam.image_height, std::vector<color>(cam.image_width));
// 	for (int k = 0; k < results.size(); ++k) {
// 		auto pixels = results[k];
// 		for (int j = 0; j < cam.image_height; ++j) {
// 			for (int i = 0; i < cam.image_width; ++i) {
// 				final_image[j][i] += pixels[j][i];
// 			}
// 		}
// 	}

// 	return final_image;
// }