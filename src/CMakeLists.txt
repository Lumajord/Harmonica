cmake_minimum_required(VERSION 2.8)

PROJECT(Harmonica)

set(CMAKE_VERBOSE_MAKEFILE ON) # verbose on


find_package(OpenGL REQUIRED)


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/kissFFT)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")


#FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
#  /usr/include/GL
#  /usr/openwin/share/include
#  /usr/openwin/include
#  /usr/X11R6/include
#  /usr/include/X11
#  /opt/graphics/OpenGL/include
#  /opt/graphics/OpenGL/contrib/libglew
#  ${CMAKE_CURRENT_SOURCE_DIR} )

FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
/usr/include
/usr/local/include
/sw/include
/opt/local/include
${GLEW_ROOT_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}
DOC "The directory where GL/glew.h resides")
set(GLEW_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/GL)


FIND_LIBRARY( GLEW_LIBRARY
NAMES GLEW glew
PATHS
/usr/lib64
/usr/lib
/usr/local/lib64
/usr/local/lib
/sw/lib
/opt/local/lib
${GLEW_ROOT_DIR}/lib
${CMAKE_CURRENT_SOURCE_DIR}/linLib/lib/x86_64)
#DOC "The GLEW library")
SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_PATH)
find_package(GLEW REQUIRED)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/linLib/lib/x86_64)
#set(PORTAUDIO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/linLib/lib/x86_64)
find_package(Portaudio REQUIRED)

else()
  #windows
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/winLib/lib)
endif()


FILE(GLOB CPP_SOURCES "*.cpp" "DspFilters/*.cpp" "kissFFT/*.c")
FILE(GLOB H_SOURCES "*.h" "DspFilters/*.h" "kissFFT/*.h")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   set(CMAKE_CXX_FLAGS "-O3 -std=c++11")
   #set(CMAKE_CXX_FLAGS "-g -std=c++11")
else()
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
endif()



ADD_EXECUTABLE(${PROJECT_NAME} ${CPP_SOURCES} ${H_SOURCES})




set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)



if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIBS ${LIBS} ${GLEW_LIBRARIES} portaudio GL glut GLU pthread) #X11 Xxf86vm Xi GL glfw3 GLEW Xrandr pthread assimp BulletDynamics BulletCollision LinearMath fmodex64 freeimage gsl gslcblas
        target_link_libraries(${PROJECT_NAME} ${LIBS})
else()
        set(LIBS ${LIBS} freeglut glew64 portaudio_x64)
        target_link_libraries(${PROJECT_NAME} ${LIBS})
endif()
