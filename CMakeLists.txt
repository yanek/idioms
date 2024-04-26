cmake_minimum_required(VERSION 3.23)

project(idioms VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD  11)
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory(vendor)
add_subdirectory(src)

target_precompile_headers(idioms PRIVATE src/pch.h)
target_link_libraries(idioms PRIVATE cglm SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static flecs::flecs_static)
