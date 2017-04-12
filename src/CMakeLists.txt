cmake_minimum_required (VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS} ${FREETYPE_LIBRARY_DIRS})

set(SOURCES gloo.cpp)

target_link_libraries(gloo ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES})

if(APPLE)
    target_compile_options(gloo PUBLIC -Wno-constant-conversion)
    target_link_libraries(gloo ${COCOA_LIBRARIES} ${IOKIT_LIBRARIES} ${COREVIDEO_LIBRARIES})
endif()

add_executable(gloo ${SOURCES})

if(APPLE)
    set_property( TARGET gloo APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

set(EXECUTABLE_OUTPUT_PATH ..)
install(TARGETS gloo DESTINATION ${gloo_SOURCE_DIR})
