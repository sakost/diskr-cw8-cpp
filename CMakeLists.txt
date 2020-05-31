cmake_minimum_required(VERSION 3.16)
project(diskr-cw8-cpp)

set(CMAKE_CXX_STANDARD 17)
set(DISKR_SOURCE_FILES mainwindow.cpp main.cpp graphmodel.cpp graphview.cpp Graph.cpp Graph.h)
set(DISKR_HEADERS_FILES mainwindow.h graphmodel.h graphview.h)
set(DISKR_RESOURCE_FILES)
set(DISKR_UI_FILES mainwindow.ui)

# for qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

unset(missingComponents)

message(STATUS "Finding Qt5")
find_package(Qt5 COMPONENTS Widgets Core Gui OpenGL REQUIRED)
if (${Qt5_FOUND})
    message(STATUS "Finding Qt5 - found")
else ()
    message(STATUS "Finding Qt5 - not found")
    list(APPEND missingComponents Qt5)
endif()
#
#message(STATUS "Finding Boost python38")
#find_package(
#        Boost
#        REQUIRED
#        COMPONENTS python38
#)
#if (Boost_FOUND)
#    message(STATUS "Finding Boost python38 - found")
#else ()
#    message(STATUS "Finding Boost python38 - not found")
#    list(APPEND missingComponents Boost)
#endif()
#
#message(STATUS "Finding Python3 headers")
#find_package(Python3 REQUIRED COMPONENTS Development)
#if (Python3_FOUND)
#    message(STATUS "Finding Python3 headers - found")
#else ()
#    message(STATUS "Finding Python3 headers - not found")
#    list(APPEND missingComponents Python3)
#endif()

list(LENGTH missingComponents compLength)

if (NOT compLength EQUAL 0)
    message(FATAL_ERROR "Missing components - ${missingComponents}")
endif ()

message(STATUS "Finding GraphViz components")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

foreach (command_to_check dot fdp sfdp neato twopi circo)
    message(STATUS "Finding ${command_to_check}")
    execute_process(COMMAND ${command_to_check} -V RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
    if(result)
        list(APPEND missingComponents ${command_to_check})
        message(STATUS "Finding ${command_to_check} - not found")
    else()
        message(STATUS "Finding ${command_to_check} - found")
    endif()
endforeach ()

list(POP_BACK CMAKE_MESSAGE_INDENT)
if(missingComponents)
    message(STATUS "Finding components - missing GraphViz components: ${missingComponents}")
    list(FIND missingComponents dot hasDot)
    if(NOT ${hasDot} EQUAL -1)
        message(FATAL_ERROR "dot was not found in PATH")
    endif()
else()
    message(STATUS "Finding GraphViz components - all components found")
endif()

unset(missingComponents)
unset(hasDot)


set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE ${CMAKE_BUILD_TYPE} )

include_directories(${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
#add_library(${PROJECT_NAME} SHARED main.cpp)
add_executable(${PROJECT_NAME} ${DISKR_SOURCE_FILES} ${DISKR_RESOURCE_FILES} ${DISKR_UI_FILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${Python3_LIBRARIES} Qt5::Gui Qt5::Widgets Qt5::Core)

