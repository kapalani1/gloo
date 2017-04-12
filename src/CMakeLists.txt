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

if(APPLE)
    set_property( TARGET gloo APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

if(APPLE)
    target_compile_options(gloo PUBLIC -Wno-constant-conversion)
    target_link_libraries(gloo ${COCOA_LIBRARIES} ${IOKIT_LIBRARIES} ${COREVIDEO_LIBRARIES})
endif()

set(EXECUTABLE_OUTPUT_PATH ..)
