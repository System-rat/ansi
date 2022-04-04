include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG origin/main
)

FetchContent_MakeAvailable(googletest)

add_executable(tests ansi_tests.cpp ../manipulators.cpp)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)