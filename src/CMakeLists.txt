cmake_minimum_required (VERSION 2.8)

set(SOURCES gloo.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

link_directories(${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS} ${FREETYPE_LIBRARY_DIRS})

add_executable(gloo ${SOURCES})
target_link_libraries(gloo glew ${GLEW_LIBRARIES} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${CMAKE_THREADS_INIT})

if(APPLE)
    set_property( TARGET gloo APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

if(APPLE)
    target_compile_options(gloo PUBLIC -Wno-constant-conversion)
    target_link_libraries(gloo ${COCOA_LIBRARIES} ${IOKIT_LIBRARIES} ${COREVIDEO_LIBRARIES})
endif()

message("42. " ${GLEW_LIBRARIES} "52. " ${GLFW_LIBRARIES} "62. " ${FREETYPE_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ..)
install(TARGETS gloo DESTINATION ${gloo_SOURCE_DIR})
