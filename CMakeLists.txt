cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROJECT_VER_MAJOR 0)
set(PROJECT_VER_MINOR 6)
set(PROJECT_VER_PATCH 0)
set(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")

add_definitions(-DPROJECT_VER="${PROJECT_VER}")
add_definitions(-DPROJECT_VER_MAJOR=${PROJECT_VER_MAJOR})
add_definitions(-DPROJECT_VER_MINOR=${PROJECT_VER_MINOR})
add_definitions(-DPROJECT_VER_PATCH=${PROJECT_VER_PATCH})

# ########### options specification ###########
option(IS_DEBUG_BUILD "Enable some debug prints, logs and checks" OFF)
option(BUILD_WITH_PRINTS_AND_LOGS "Enable printing log messages" OFF)
option(USE_CONSOLE "Enable console with debugging commands" OFF)

option(BUILD_TESTS "BUILD TESTS" OFF)
option(RUN_LINTING "RUN_CLANG_TIDY linting" ON)
option(RUN_STATIC_ANALYSIS "RUN_STATIC_ANALYSIS TESTS" OFF)

# ########### options and definitions ###########
if(${IS_DEBUG_BUILD})
    message(WARNING "!!! Building program in debug mode!")
    add_definitions(-DIS_DEBUG_BUILD)
endif()

if(${BUILD_WITH_PRINTS_AND_LOGS})
    message(WARNING "!!! Building program with prints and logs!")
    add_definitions(-DENABLE_PRINTS_AND_LOGS)
endif()

if(${USE_CONSOLE})
    message(WARNING "!!! Building with console!")
    add_definitions(-DUSE_CONSOLE)
endif()

# ########### Build process cmake inclusion ###########
if(${BUILD_TESTS} OR ${RUN_STATIC_ANALYSIS})
    set(TESTS_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(${TESTS_CMAKE})
else()
    if(DEFINED ENV{IDF_PATH})
        set(IDF_PATH $ENV{IDF_PATH})
    else()
        set(IDF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/app/externals/esp-idf")
    endif()

    set(IDF_CMAKE ${IDF_PATH}/tools/cmake/project.cmake)
    set(EXTRA_COMPONENT_DIRS app/ ${EXTRA_COMPONENT_DIRS})
    set(SUPPORTED_TARGETS esp32)
    include(${IDF_CMAKE})
endif()

set(MARKER_SEQUENCE "^x^") # it is a sequence to easily match the parameter from bash
add_custom_target(print_version
    COMMAND echo "${MARKER_SEQUENCE}"
    COMMAND echo "${PROJECT_VER}")

add_definitions(-DIS_ESP)

project(sparkhub-LevelSense)
