#CMakeLists.txt

#minimum version for CMake
cmake_minimum_required(VERSION 3.10)
#get mode = debug or release
set(mode $ENV{mode})
#Set the project name
project(stockExchange)

#set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++14")
# Set build type
if(mode MATCHES debug)
	message(STATUS "compilation mode is DEBUG!")
	set(CMAKE_BUILD_TYPE Debug)
endif()
#Add an executable
add_executable(sExch stockmain.cpp)
