cmake_minimum_required(VERSION 3.19)
project(aoc2023)

find_package(fmt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Werror -stdlib=libc++")

function(aoc_day_solution day_id)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/solution.cpp")
    else ()
        file(MAKE_DIRECTORY ${day_id})
        file(TOUCH ${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/data.in)
        configure_file(template.in ${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/solution.cpp COPYONLY)
    endif ()
    add_executable(${day_id} ${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/solution.cpp)
    target_compile_definitions(${day_id} PUBLIC -DDAY_PATH=\"../${day_id}/data.in\")
    target_link_libraries(${day_id} fmt::fmt-header-only -lpthread -l:libc++.a)
endfunction()

aoc_day_solution(1)
aoc_day_solution(2)
aoc_day_solution(3)