cmake_minimum_required (VERSION 2.8)
project (gloo)
set(CMAKE_CXX_FLAGS "-Wall -std=gnu++11 -m64 -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

find_package (OpenGL REQUIRED)

if (APPLE)
    find_package (GLEW REQUIRED)
    find_package (glfw3 3.2 REQUIRED)
    set(GLFW_LIBRARIES "glfw")
else ()
    find_package (Threads REQUIRED)
    find_package (X11 REQUIRED)
    if(NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "Xrandr library not found - required for GLFW")
    endif()

    if(NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "xf86vmode library not found - required for GLFW")
    endif()

    if(NOT X11_Xcursor_FOUND)
        message(FATAL_ERROR "Xcursor library not found - required for GLFW")
    endif()

    if(NOT X11_Xinerama_FOUND)
        message(FATAL_ERROR "Xinerama library not found - required for GLFW")
    endif()

    if(NOT X11_Xi_FOUND)
        message(FATAL_ERROR "Xi library not found - required for GLFW")
    endif()
    list(APPEND GLFW_x11_LIBRARY "${X11_Xrandr_LIB}" "${X11_Xxf86vm_LIB}" "${X11_Xcursor_LIB}" "${X11_Xinerama_LIB}" "${X11_Xi_LIB}" "${X11_LIBRARIES}" "${CMAKE_THREAD_LIBS_INIT}" -lrt -ldl)

    find_path(GLFW_INCLUDE_DIRS NAMES GLFW/glfw3.h PATHS "${PROJECT_SOURCE_DIR}/include")
    find_library(GLFW_glfw_LIBRARY NAMES libglfw3.a PATHS "${PROJECT_SOURCE_DIR}/lib")
    set(GLFW_LIBRARIES "${GLFW_glfw_LIBRARY}" "${GLFW_x11_LIBRARY}")

    find_path(GLEW_INCLUDE_DIRS NAMES GL/glew.h PATHS "${PROJECT_SOURCE_DIR}/include")
    find_library(GLEW_LIBRARIES NAMES libGLEW.a PATHS "${PROJECT_SOURCE_DIR}/lib")

endif (APPLE)
add_subdirectory(src)

