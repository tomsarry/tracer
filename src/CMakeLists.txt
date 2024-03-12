set(BINARY ${CMAKE_PROJECT_NAME})

enable_testing()

add_executable(tracer main.cpp)

set_target_properties(tracer PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(external)
add_subdirectory(materials)
add_subdirectory(objects)
add_subdirectory(textures)
add_subdirectory(utils)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(tracer PUBLIC
	tracer.external
	tracer.materials
	tracer.objects
	tracer.textures
	tracer.utils
	${CURSES_LIBRARY}
)