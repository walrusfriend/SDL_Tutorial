cmake_minimum_required(VERSION 3.7)

project(SDL_Tutorial)

# How to include SDL2:
# 1. Download FindSDL2.cmake file (google) and place it in any folder (/cmake, for example)
# 2. Point a folder with FindSDL2.cmake to cmake via CMAKE_MODULE_PATH variable
# 3. Set the SDL2 installed directory path to SDL2_DIR variable
# 3. Find package SDL2
# 4. Include SDL2_INCLUDE_DIRS and target_link_libraries SDL2_LIBRARIES

# Include FindSDL2.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SDL2_DIR C:/libs/SDL2)
set(SDL2_image_DIR C:/libs/SDL2)

# Point to source files
file(GLOB SOURCES "src/*.cpp")

# Point to header files
file(GLOB HEADERS "headres/*.h")
include_directories(${HEADERS})

# Include TTF module
set(SDL_ttf_LIBRARY C:/libs/SDL2/lib/x64/SDL2_ttf.lib)

# Try to find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

add_executable(SDL_Tutorial ${SOURCES})
set_target_properties(SDL_Tutorial PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(SDL_Tutorial PUBLIC ${HEADERS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL_ttf_LIBRARY})