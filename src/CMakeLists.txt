cmake_minimum_required (VERSION 2.8)

set(SOURCES gloo.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(gloo ${SOURCES})
target_link_libraries(gloo 
                      ${GLEW_LIBRARIES} 
                      ${GLFW_LIBRARIES} 
                      ${OPENGL_LIBRARIES} 
                      ${FREETYPE_LIBRARIES} 
                      ${CMAKE_THREADS_INIT})

set(EXECUTABLE_OUTPUT_PATH ..)
