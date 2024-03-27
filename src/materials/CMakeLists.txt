set(SOURCES
    dielectric.h
    diffuse_light.h
    iridescent.h
    lambertian.h
    material.h
    metal.h
    scratched_metal.h
)

add_library(tracer.materials ${SOURCES})

set_target_properties(tracer.materials PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(tracer.materials PUBLIC
    tracer.utils
)
