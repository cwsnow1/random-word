cmake_minimum_required(VERSION 3.5)

project(generator)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -std=c++23 -fno-exceptions -fno-rtti -fno-omit-frame-pointer -Wno-unused-parameter")

add_library(${PROJECT_NAME}lib
    ${PROJECT_SOURCE_DIR}/american_english.cpp
    ${PROJECT_SOURCE_DIR}/metropolitan_french.cpp
    ${PROJECT_SOURCE_DIR}/phonology.cpp
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    ${PROJECT_NAME}lib
)

if (BUILD_BENCHMARK)
    find_package(benchmark REQUIRED)
    include_directories(SYSTEM benchmark/include)
    add_executable(${PROJECT_NAME}_BM
        ${PROJECT_SOURCE_DIR}/bench.cpp
    )
    target_link_libraries(${PROJECT_NAME}_BM
        ${PROJECT_NAME}lib
        benchmark::benchmark
        pthread
    )
endif()