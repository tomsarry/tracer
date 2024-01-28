set(BINARY ${CMAKE_PROJECT_NAME})

enable_testing()

add_executable(tracer main.cpp)

set_target_properties(tracer PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(materials)
add_subdirectory(objects)
add_subdirectory(utils)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(tracer PUBLIC
	tracer.objects
	tracer.utils
	${CURSES_LIBRARY}
)