cmake_minimum_required(VERSION 3.9.4)
project(HelloWorld)
file(GLOB SOURCES "*.cpp")
add_executable(hello ${SOURCES})
