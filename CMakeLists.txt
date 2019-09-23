cmake_minimum_required(VERSION 3.4)
project(StarMapRecognition)

################################################################################
# OSG
find_package(OpenSceneGraph REQUIRED osg osgDB osgViewer osgGA osgText)
if (${OPENSCENEGRAPH_FOUND})
    message(STATUS "OSG_INCLUDE_DIR:${OSG_INCLUDE_DIR}")
    message(STATUS "OPENSCENEGRAPH_LIBRARIES:${OPENSCENEGRAPH_LIBRARIES}")
else ()
    message(FATAL_ERROR "NOT found osg.")
endif ()
set(OSG_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES})
include_directories(${OSG_INCLUDE_DIR})

################################################################################
# Boost
#if (DEFINED BOOST_ROOT)
find_package(Boost COMPONENTS system log_setup log)
if (${Boost_FOUND})
    message(STATUS "Boost_INCLUDE_DIR:${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_LIBRARY_DIRS:${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_LIBRARIES:${Boost_LIBRARIES}")
else ()
    message(FATAL_ERROR "Can NOT find boost libraries. (BOOST_ROOT=${BOOST_ROOT})")
endif ()
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)
#else()
#    message(FATAL_ERROR "Please SET BOOST_ROOT in CustomConfig.cmake")
#endif()

################################################################################
# pugixml
#set(pugixml_DIR "/usr/share/libpugixml-dev/cmake")
#find_package(pugixml)
#if (${pugixml_FOUND})
#    message(STATUS "Found pugixml.")
#    set(PUGIXML_LIBRARY pugixml)
#else ()
message(STATUS "find_package(pugixml) failed, using custom set.")
#    set(pugixml_INCLUDE_DIR /usr/include)
find_path(PUGIXML_INCLUDE_DIR pugixml.hpp)
if (EXISTS ${PUGIXML_INCLUDE_DIR})
    if (EXISTS ${PUGIXML_INCLUDE_DIR}/pugixml.hpp)
        message(STATUS "PUGIXML_INCLUDE_DIR:${PUGIXML_INCLUDE_DIR}")
    else ()
        message(FATAL_ERROR "Can NOT find pugixml.hpp. (PUGIXML_INCLUDE_DIR=${PUGIXML_INCLUDE_DIR})")
    endif ()
    include_directories(${PUGIXML_INCLUDE_DIR})
    # header only mode
    #    add_definitions(-DPUGIXML_HEADER_ONLY=1)
    find_library(PUGIXML_LIBRARY pugixml)
    message(STATUS "PUGIXML_LIBRARY:${PUGIXML_LIBRARY}")
else ()
    message(FATAL_ERROR "Please SET PUGIXML_INCLUDE_DIR in CustomConfig.cmake")
endif ()
#endif ()

################################################################################
# Ceres
find_package(Ceres)
if (${Ceres_FOUND})
    message(STATUS "CERES_INCLUDE_DIRS:${CERES_INCLUDE_DIRS}")
    message(STATUS "CERES_LIBRARIES:${CERES_LIBRARIES}")
else ()
    message(FATAL_ERROR "Can NOT find Ceres.")
endif ()

################################################################################
# Eigen3
find_package(Eigen3)
if (${Eigen3_FOUND})
    message(STATUS "EIGEN3_DEFINITIONS:${EIGEN3_DEFINITIONS}")
    message(STATUS "EIGEN3_INCLUDE_DIRS:${EIGEN3_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Can NOT find Eigen3.")
endif ()
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

################################################################################
# OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_LIBS:${OpenCV_LIBS}")
message(STATUS "OpenCV_INCLUDE_DIRS:${OpenCV_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(src)
