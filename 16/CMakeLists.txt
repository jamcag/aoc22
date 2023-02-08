cmake_minimum_required(VERSION 3.16)

project(cpp)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.2.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests test_state.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
