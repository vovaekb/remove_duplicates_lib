cmake_minimum_required(VERSION 3.16.0)

project(remove_duplicates_lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -O2 -lopencv_imgproc")
# -Wall

# OpenCV

find_package(OpenCV REQUIRED)
set(OPENCV_ENABLE_NONFREE "Enable non-free algorithms" ON)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

# Boost

find_package(Boost)
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})

# Conan

message( "CMAKE_BINARY_DIR : " ${CMAKE_BINARY_DIR} )
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup(TARGETS)

include_directories(${CONAN_INCLUDE_DIRS} "include")

# add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(
        ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} ${Boost_LIBRARIES} opencv_core opencv_highgui opencv_imgproc)

# add_executable(test_app src/main.cpp)
# target_link_libraries(test_app ${OpenCV_LIBS} ${Boost_LIBRARIES})

# install
install(TARGETS ${PROJECT_NAME} DESTINATION lib) 
install(DIRECTORY include/ DESTINATION include
        FILES_MATCHING PATTERN "*.h")

# add tests
add_subdirectory(test)