set(SOURCES
    color.h
    constants.h
    ray.h
    vec3.h
)

add_library(tracer.utils ${SOURCES})

set_target_properties(tracer.utils PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
