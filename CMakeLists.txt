cmake_minimum_required(VERSION 3.14)
project(SimplePhysicsEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)


set(SOURCES
    src/main.cpp
    # если появятся другие .cpp — допиши сюда:
    # src/simple_engine.cpp
    # src/material_point.cpp
)


add_executable(simple_engine ${SOURCES})


target_include_directories(simple_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================================
# 5. Линкуем raylib
# ============================================================
target_link_libraries(simple_engine PRIVATE raylib)