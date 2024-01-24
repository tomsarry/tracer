set(SOURCES main.cpp tst_vec3.cpp)

add_executable(
	tracer.utils.test
	main.cpp
	tst_ray.cpp
	tst_vec3.cpp
)
target_link_libraries(
	tracer.utils.test PUBLIC
	tracer.utils
	GTest::gtest
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tracer.utils.test)