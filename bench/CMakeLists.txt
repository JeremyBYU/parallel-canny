
include(FetchContent)
# This module depend on having doctest installed
FetchContent_Declare(
  extern_doctest
  GIT_REPOSITORY https://github.com/onqtam/doctest.git
  GIT_TAG        2.3.7
  GIT_SHALLOW TRUE
)

FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(extern_doctest)
FetchContent_MakeAvailable(nanobench)


add_executable(run-bench canny-bench run-bench.cpp)
target_include_directories(run-bench PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty)
target_link_libraries(run-bench PRIVATE doctest::doctest nanobench PCanny::PCAN)