#include <curses.h>

#include "objects/hittable_list.h"
#include "utils/camera.h"
#include "utils/concurrency.h"
#include "utils/io.h"
#include "utils/pixel_map.h"
#include "utils/scenes.h"

const ThreadNumber N_THREADS = ThreadNumber::ONE;
const bool REAL_TIME = true;

void one_shot_render(camera& cam, hittable_list& world) {
	pixel_map final_image = multi_thread_render(N_THREADS, cam, world);

	io::display(
		final_image, cam.image_height, cam.image_width,
		cam.samples_per_pixel * N_THREADS);
}

bool handle_user_input(camera& cam, char input) {
	const auto delta = 0.5;
	switch (input) {
		case ERR:  // do nothing
			return true;
		case 'q':  // quit
			return false;
		case 'a':
			cam.look_from += vec3(-0.5, 0, 0);
			cam.look_at += vec3(-0.5, 0, 0);
			return true;
		case 'd':
			cam.look_from += vec3(0.5, 0, 0);
			cam.look_at += vec3(0.5, 0, 0);
			return true;
		case '+':
			cam.zoom(5.0);
			return true;
		case '-':
			cam.zoom(-5.0);
			return true;
		default:
			std::clog << "Unrecognized char: " << input << std::endl;
	}

	return true;
}

void real_time_render(camera& cam, hittable_list& world) {
	bool should_continue = true;
	initscr();
	cbreak();

	while (should_continue) {
		pixel_map frame = multi_thread_render(N_THREADS, cam, world);

		io::write_to_file(
			"real_time_render.ppm", frame, cam.image_height, cam.image_width,
			cam.samples_per_pixel * N_THREADS);

		auto res = getch();
		should_continue = handle_user_input(cam, res);
	}

	endwin();
}

int main() {
	hittable_list world;
	camera cam;

	scenes::book_one::simple_scene_rrt(world, cam, 2, 10);

	real_time_render(cam, world);
	return 0;
}
