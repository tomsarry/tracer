#ifndef SCENES_H
#define SCENES_H

#include "objects/hittable_list.h"
#include "utils/camera.h"

namespace scenes::book_one {

void end(hittable_list& world, camera& cam);

}  // namespace scenes::book_one

#endif