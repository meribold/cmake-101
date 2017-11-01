cmake_minimum_required(VERSION 3.9.4)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 14)

# Enable some warnings.  See <https://stackoverflow.com/a/14235055>.
if (CMAKE_COMPILER_IS_GNUCC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif (CMAKE_COMPILER_IS_GNUCC)
if (MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif (MSVC)

file(GLOB SOURCES "src/*.cpp")
add_executable(hello ${SOURCES})
find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(hello Boost::filesystem)
