cmake_minimum_required(VERSION 3.10.2)

################################################################################
# vcpkg
################################################################################
include (${CMAKE_SOURCE_DIR}/cmake/vcpkg.cmake)

################################################################################
# Project
################################################################################
project(SolitaireGame VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

################################################################################
# External libraries
################################################################################
find_package(glog CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

################################################################################
# Sources
################################################################################
set(coregame_dir src/CoreGame/)
set(engine_dir src/Engine/)
set(utils_dir src/Utils/)

file(GLOB_RECURSE common_sources ${engine_dir}*.cpp ${utils_dir}*.cpp ${coregame_dir}*.cpp)
file(GLOB_RECURSE common_headers ${engine_dir}*.h ${utils_dir}*.h ${coregame_dir}*.h)
file(GLOB_RECURSE src_sources src/*.cpp)
file(GLOB_RECURSE src_headers src/*.h)

set(sources ${src_sources} ${src_headers} ${common_sources} ${common_headers})

################################################################################
# Executable
################################################################################
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glog::glog
    GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
    sfml-system sfml-graphics sfml-window
)

################################################################################
# Resources
################################################################################
include (${CMAKE_SOURCE_DIR}/cmake/copy_res.cmake)
################################################################################
# - Blueprints
################################################################################

set(BLUEPRINTS_DIR Blueprints)
# file(GLOB blueprints ${BLUEPRINTS_DIR}/*.*)
CopyResources(${BLUEPRINTS_DIR}/*.*  $<TARGET_FILE_DIR:${PROJECT_NAME}>)

################################################################################
# - Fonts
################################################################################
set(FONT_DIR Font)
# file(GLOB fonts ${FONT_DIR}/*.*)
CopyResources(${FONT_DIR}/*.* $<TARGET_FILE_DIR:${PROJECT_NAME}>)

################################################################################
# SubProjects
################################################################################
add_subdirectory(SolitaireTest)
