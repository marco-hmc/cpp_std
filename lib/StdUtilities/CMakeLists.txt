cmake_minimum_required(VERSION 3.1.0)

project(StdUtilities VERSION 0.1.0)

file(GLOB LIB_SOURCES "src/*.cpp")
include_directories("include")
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})

find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

# set(PARENT_PROJECT_NAME ${PROJECT_NAME})
# add_subdirectory(test)
