cmake_minimum_required(VERSION 2.8)

if(APPLE)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
        include_directories( "/System/Library/Frameworks" )
        find_library (COCOA_LIBRARIES Cocoa)
        find_library (IOKIT_LIBRARIES IOkit)
        find_library (COREVIDEO_LIBRARIES CoreVideo)
    
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
            set(CLANG_CXX_FLAGS "-std=c++11 -m64")
            set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -O3")
            set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -funroll-loops")
            set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -Wno-narrowing")
            set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -Wno-deprecated-register")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")
        endif()

        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            set(GCC_CXX_FLAGS "-std=gnu++11 -m64")
            set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -O3")
            set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -fopenmp")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")
        endif()

    endif()

endif(APPLE)

##################
# Build on Linux #
##################
set(LINUX UNIX AND NOT APPLE)
if(LINUX)

    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        set(GCC_CXX_FLAGS "-std=gnu++11 -m64")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXi")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXxf86vm")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXinerama")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXcursor")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXfixes")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrandr")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXext")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrender")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lX11")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lpthread")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lxcb")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXau")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -O3")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -fopenmp")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")
    endif()

endif(LINUX)

find_package(OpenGL REQUIRED)
find_package(FreeType REQUIRED)
find_package(PkgConfig QUIET)

if(PKGCONFIG_FOUND)
    pkg_search_module(GLEW QUIET GLEW)
else(PKGCONFIG_FOUND)
    find_package(GLEW QUIET)
endif()
if(NOT GLEW_FOUND)
    add_subdirectory("glew")
    set_property( TARGET glew APPEND_STRING PROPERTY COMPILE_FLAGS -w )
endif()

# GLFW
if(PKGCONFIG_FOUND)
    pkg_search_module(GLFW QUIET glfw3)
else(PKGCONFIG_FOUND)
    find_package(GLFW QUIET)
endif()
if(NOT GLFW_FOUND)
    add_subdirectory("glfw")
    set_property( TARGET glfw APPEND_STRING PROPERTY COMPILE_FLAGS -w )
endif()

include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS} ${FREETYPE_LIBRARY_DIRS})
