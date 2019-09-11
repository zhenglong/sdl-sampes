cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "HelloWorld" )

## Set our project name
project(${PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Debug")

include_directories("/use/local/include")
link_directories("/usr/local/lib")
link_libraries(SDL2 SDL2_image sndfile)

set(CMAKE_CXX_FLAGS "-g")

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS "main.c")

## Define the executable
add_executable(${PROJECT_NAME} ${SRCS})
