set(SOURCES
    camera.h
    color.h
    concurrency.cpp
    concurrency.h
    constants.h
    interval.h
    pixel_map.h
    point2.h
    random.h
    ray.h
    scenes.cpp
    scenes.h
    vec3.h
)

add_library(tracer.utils ${SOURCES})

set_target_properties(tracer.utils PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
