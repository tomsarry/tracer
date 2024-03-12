set(SOURCES
    stb_image.h
)

add_library(tracer.external ${SOURCES})

set_target_properties(tracer.external PROPERTIES LINKER_LANGUAGE CXX)
