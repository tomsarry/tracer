#include "objects/hittable_list.h"
#include "utils/camera.h"
#include "utils/concurrency.h"
#include "utils/io.h"
#include "utils/pixel_map.h"
#include "utils/scenes.h"

const ThreadNumber N_THREADS = ThreadNumber::FOUR;

int main() {
	hittable_list world;
	camera cam;

	scenes::book_one::end(world, cam, 32, 10);

	pixel_map final_image = multi_thread_render(N_THREADS, cam, world);

	io::display(
		final_image, cam.image_height, cam.image_width,
		cam.samples_per_pixel * N_THREADS);
}
