cmake_minimum_required(VERSION 3.16)
project(potential_broccoli)

file(GLOB SOURCE "src/*.h" "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCE})