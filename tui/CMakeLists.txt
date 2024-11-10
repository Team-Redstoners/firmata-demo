cmake_minimum_required(VERSION 3.25)

project(main)

file(GLOB_RECURSE source_files src/*.c)
file(GLOB_RECURSE header_files src/*.h)

add_executable(main ${source_files} ${header_files})

target_compile_options(main PUBLIC -Wall -Werror -Wextra)

target_link_libraries(main ncurses)
