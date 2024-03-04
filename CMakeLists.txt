cmake_minimum_required(VERSION 3.27)
project(KGit)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_EXAMPLES *.cpp *.h)
add_executable(${PROJECT_NAME} ${SRC_EXAMPLES})

set(GIT2_INCLUDE_DIR "C:/Program Files (x86)/libgit2/include")
set(GIT2_LIBRARY_DIR "C:/Program Files (x86)/libgit2/lib")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
find_package(libgit2 REQUIRED)
include_directories(${GIT2_INCLUDE_DIR})

add_definitions(-DGIT_DEPRECATE_HARD)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GIT2_LIBRARIES})
