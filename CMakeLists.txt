cmake_minimum_required(VERSION 3.10)
project(projetMario)

set(CMAKE_C_STANDARD 99)

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2")

include_directories(${SDL2_DIR}/include)
link_directories(${SDL2_DIR}/lib)

add_executable(projetMario main.c Affichage.c deplacement.c)

target_link_libraries(projetMario SDL2main SDL2 winmm)