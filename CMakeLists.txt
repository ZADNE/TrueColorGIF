cmake_minimum_required(VERSION 3.14)
project(TrueColorGIF)

#lodepng
add_library(lodepng STATIC)
target_include_directories(lodepng PRIVATE .)
set_target_properties(lodepng PROPERTIES CXX_STANDARD 20)
add_subdirectory(lodepng)

#TrueColorGIF
add_executable(TrueColorGIF)
target_include_directories(TrueColorGIF PRIVATE .)
set_target_properties(TrueColorGIF PROPERTIES CXX_STANDARD 20)
target_link_libraries(TrueColorGIF PRIVATE lodepng)
add_subdirectory(TrueColorGIF)
