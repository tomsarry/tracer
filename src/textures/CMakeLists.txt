set(SOURCES
    texture.h
)

add_library(tracer.textures ${SOURCES})

set_target_properties(tracer.textures PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(tracer.textures PUBLIC
    tracer.utils
)
