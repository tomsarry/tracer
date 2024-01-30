#include "concurrency.h"

#include <future>
#include <memory>

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

void display_info_header(const camera& cam, int threads) {
	std::cerr << "Rendering " << cam.image_width << "x" << cam.image_height
			  << std::endl;
	std::cerr << "Samples/pixel " << cam.samples_per_pixel << " on " << threads
			  << " threads "
			  << "(" << cam.samples_per_pixel / threads << ")" << std::endl;
}

pixel_map render_using_one_thread(camera& cam, const hittable_list& world) {
	display_info_header(cam, 1);

	pixel_map pixels(cam.image_height, std::vector<color>(cam.image_width));

	cam.render(std::make_shared<hittable_list>(world), pixels);

	return pixels;
}

pixel_map render_using_four_threads(camera& cam, const hittable_list& world) {
	display_info_header(cam, 4);

	cam.samples_per_pixel /= 4;

	const auto height = cam.image_height;
	const auto width = cam.image_width;

	pixel_map pixels1(height, std::vector<color>(width));
	auto render1 = [&]() { cam.render(world.deep_copy(), pixels1); };

	pixel_map pixels2(height, std::vector<color>(width));
	auto render2 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels2);
	};

	pixel_map pixels3(height, std::vector<color>(width));
	auto render3 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels3);
	};

	pixel_map pixels4(height, std::vector<color>(width));
	auto render4 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels4);
	};

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

pixel_map render_using_six_threads(camera& cam, const hittable_list& world) {
	display_info_header(cam, 6);

	cam.samples_per_pixel /= 6;

	const auto height = cam.image_height;
	const auto width = cam.image_width;

	pixel_map pixels1(height, std::vector<color>(width));
	auto render1 = [&]() {
		cam.render(std::make_shared<hittable_list>(world), pixels1);
	};

	pixel_map pixels2(height, std::vector<color>(width));
	auto render2 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels2);
	};

	pixel_map pixels3(height, std::vector<color>(width));
	auto render3 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels3);
	};

	pixel_map pixels4(height, std::vector<color>(width));
	auto render4 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels4);
	};

	pixel_map pixels5(height, std::vector<color>(width));
	auto render5 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels5);
	};

	pixel_map pixels6(height, std::vector<color>(width));
	auto render6 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels6);
	};

	auto handle1 = std::async(std::launch::async, render1);
	auto handle2 = std::async(std::launch::async, render2);
	auto handle3 = std::async(std::launch::async, render3);
	auto handle4 = std::async(std::launch::async, render4);
	auto handle5 = std::async(std::launch::async, render5);
	auto handle6 = std::async(std::launch::async, render6);

	handle1.wait();
	handle2.wait();
	handle3.wait();
	handle4.wait();
	handle5.wait();
	handle6.wait();

	add_pixel_maps(pixels1, pixels2, width, height);
	add_pixel_maps(pixels1, pixels3, width, height);
	add_pixel_maps(pixels1, pixels4, width, height);
	add_pixel_maps(pixels1, pixels5, width, height);
	add_pixel_maps(pixels1, pixels6, width, height);

	return pixels1;
}

pixel_map render_using_eight_threads(camera& cam, const hittable_list& world) {
	display_info_header(cam, 8);

	cam.samples_per_pixel /= 8;

	const auto height = cam.image_height;
	const auto width = cam.image_width;

	pixel_map pixels1(height, std::vector<color>(width));
	auto render1 = [&]() {
		cam.render(std::make_shared<hittable_list>(world), pixels1);
	};

	pixel_map pixels2(height, std::vector<color>(width));
	auto render2 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels2);
	};

	pixel_map pixels3(height, std::vector<color>(width));
	auto render3 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels3);
	};

	pixel_map pixels4(height, std::vector<color>(width));
	auto render4 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels4);
	};

	pixel_map pixels5(height, std::vector<color>(width));
	auto render5 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels5);
	};

	pixel_map pixels6(height, std::vector<color>(width));
	auto render6 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels6);
	};

	pixel_map pixels7(height, std::vector<color>(width));
	auto render7 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels7);
	};

	pixel_map pixels8(height, std::vector<color>(width));
	auto render8 = [&]() {
		cam.deep_copy()->render(world.deep_copy(), pixels8);
	};

	auto handle1 = std::async(std::launch::async, render1);
	auto handle2 = std::async(std::launch::async, render2);
	auto handle3 = std::async(std::launch::async, render3);
	auto handle4 = std::async(std::launch::async, render4);
	auto handle5 = std::async(std::launch::async, render5);
	auto handle6 = std::async(std::launch::async, render6);
	auto handle7 = std::async(std::launch::async, render7);
	auto handle8 = std::async(std::launch::async, render8);

	handle1.wait();
	handle2.wait();
	handle3.wait();
	handle4.wait();
	handle5.wait();
	handle6.wait();
	handle7.wait();
	handle8.wait();

	add_pixel_maps(pixels1, pixels2, width, height);
	add_pixel_maps(pixels1, pixels3, width, height);
	add_pixel_maps(pixels1, pixels4, width, height);
	add_pixel_maps(pixels1, pixels5, width, height);
	add_pixel_maps(pixels1, pixels6, width, height);
	add_pixel_maps(pixels1, pixels7, width, height);
	add_pixel_maps(pixels1, pixels8, width, height);

	return pixels1;
}

pixel_map multi_thread_render(
	ThreadNumber t, camera& cam, const hittable_list& world) {
	switch (t) {
		case ThreadNumber::ONE:
			return render_using_one_thread(cam, world);
		case ThreadNumber::FOUR:
			return render_using_four_threads(cam, world);
		case ThreadNumber::SIX:
			return render_using_six_threads(cam, world);
		case ThreadNumber::EIGHT:
			return render_using_eight_threads(cam, world);
	}

	assert(false);
}
