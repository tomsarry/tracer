set(SOURCES
    hittable.h
    hittable_list.h
    quad.h
    sphere.h
)

add_library(tracer.objects ${SOURCES})

set_target_properties(tracer.objects PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(tracer.objects PUBLIC
    tracer.utils
)
