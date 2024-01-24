set(SOURCES vec3.h)

add_library(tracer.utils ${SOURCES})

include(GoogleTest)

set_target_properties(tracer.utils PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
