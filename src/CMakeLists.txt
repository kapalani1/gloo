cmake_minimum_required (VERSION 2.8)

set(SOURCES gloo.cpp)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
add_executable(gloo ${SOURCES})
target_link_libraries(gloo ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH ..)
