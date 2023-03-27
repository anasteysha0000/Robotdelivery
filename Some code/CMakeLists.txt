cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_PREFIX_PATH "C:/lib/OpenCV/install/opencv/x64/vc17/lib")
set(PROJECT_NAME "mai")

project(${PROJECT_NAME})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp Color.h Spot.h Camera.h)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

cmake_path(GET OpenCV_DIR PARENT_PATH OpenCV_PARENT_DIR)
set(OpenCV_PARENT_DIR ${OpenCV_PARENT_DIR}/bin/)

file(GLOB DLLS RELATIVE "${OpenCV_PARENT_DIR}" "${OpenCV_PARENT_DIR}/*.dll")
foreach(DLL ${DLLS})
    add_custom_command(
            TARGET ${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND}
            -E copy "${OpenCV_PARENT_DIR}/${DLL}" "${CMAKE_BINARY_DIR}/${DLL}"
    )
endforeach()