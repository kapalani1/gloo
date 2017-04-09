cmake_minimum_required (VERSION 3.2)

set(SOURCES gloo.cpp)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
add_executable(gloo ${SOURCES})
target_link_libraries(gloo glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH ..)
