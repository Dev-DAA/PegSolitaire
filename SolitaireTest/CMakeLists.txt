cmake_minimum_required(VERSION 3.10.2)

################################################################################
# Project
################################################################################
project(SolitaireTest)

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
set(engine_dir_test ../src/Engine/)
set(utils_dir_test ../src/Utils/)

file(GLOB_RECURSE common_sources_test ${engine_dir_test}*.cpp ${utils_dir_test}*.cpp)  
file(GLOB_RECURSE common_headers_test ${engine_dir_test}*.h ${utils_dir_test}*.h)

file(GLOB_RECURSE TEST_SRC_SOURCE ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE TEST_SRC_HEADERS ${PROJECT_SOURCE_DIR}/*.h)

set(sources_test
    ${TEST_SRC_SOURCE}
    ${TEST_SRC_HEADERS}
    ${common_sources_test}
    ${common_headers_test}
)

################################################################################
# Executable
################################################################################
add_executable(testmain ${sources_test})
target_link_libraries(testmain PRIVATE 
    glog::glog
    GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
    sfml-system sfml-graphics sfml-window
)

################################################################################
# Tests
################################################################################
enable_testing()
add_test(NAME testmain COMMAND testmain)