
add_executable(canny-sp ${CMAKE_CURRENT_SOURCE_DIR}/canny_serial.cpp)
target_include_directories(canny-sp PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty)