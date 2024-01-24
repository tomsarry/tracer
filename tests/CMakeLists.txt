set(SOURCES main.cpp tst_vec3.cpp)

enable_testing()

add_executable(
    tracer.test
    main.cpp
    tst_vec3.cpp
)
target_link_libraries(
    tracer.test
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tracer.test)