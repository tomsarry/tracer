set(BINARY ${CMAKE_PROJECT_NAME})

enable_testing()

add_executable(tracer main.cpp)

set_target_properties(tracer PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(objects)
add_subdirectory(utils)

target_link_libraries(tracer PUBLIC
	tracer.objects
	tracer.utils
)