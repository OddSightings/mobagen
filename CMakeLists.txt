
cmake_minimum_required(VERSION 3.16.1)
include(cmake/gate/cmake/HunterGate.cmake)
option(HUNTER_STATUS_DEBUG "Hunter debug info" ON)
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz"
        SHA1 "64024b7b95b4c86d50ae05b926814448c93a70a0"
        LOCAL
)


# CPM downloader
include(external/cpm.cmake)

# project declaration
project(MoBaGEn)

# standands
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# todo: fix this error if the c standard is required
# error: EM_ASM does not work in -std=c* modes, use -std=gnu* modes instead
#set(CMAKE_C_STANDARD 17)
#set(CMAKE_C_EXTENSIONS OFF)

# set output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set linkage
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/libs)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/libs/Debug)

IF(EMSCRIPTEN)
    message(STATUS "emscripten build")
    #  SET(CMAKE_CXX_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets/ -s WASM=0 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=2 -s USE_PTHREADS=1 --proxy-to-worker -s FETCH=1")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets/ -s WASM=0 -s DISABLE_EXCEPTION_CATCHING=2 --proxy-to-worker -s FETCH=1")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets/")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -s WASM=0 -s FULL_ES2=1 -s USE_SDL=0 -s EXPORT_ALL=1")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -s WASM=0 -s FULL_ES2=1 -s USE_SDL=0 -s EXPORT_ALL=1")

    add_definitions(-DASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/html/index.html ${CMAKE_BINARY_DIR}/bin/index.html COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/html/catchthecat.html ${CMAKE_BINARY_DIR}/bin/catchthecat.html COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/html/flocking.html ${CMAKE_BINARY_DIR}/bin/flocking.html COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/html/maze.html ${CMAKE_BINARY_DIR}/bin/maze.html COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/html/scenario.html ${CMAKE_BINARY_DIR}/bin/scenario.html COPYONLY)
ENDIF()

add_subdirectory(external)
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(examples)