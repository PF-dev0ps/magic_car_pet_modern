cmake_minimum_required(VERSION 3.16)
project(magic_car_pet_modern LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
add_executable(fly
    src/fly.cpp
    src/mcpet.cpp
    src/tpf.cpp
    src/grp_sdl.cpp
    src/keyboard_sdl.cpp
)
target_include_directories(fly PRIVATE src ${SDL2_INCLUDE_DIRS})
target_link_libraries(fly PRIVATE ${SDL2_LIBRARIES})

add_custom_command(TARGET fly POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:fly>/assets)
