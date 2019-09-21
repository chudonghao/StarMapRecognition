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
find_package(Boost COMPONENTS system)
if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIR:${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_LIBRARY_DIRS:${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_LIBRARIES:${Boost_LIBRARIES}")
else ()
    message(FATAL_ERROR "Can NOT find boost libraries. (BOOST_ROOT=${BOOST_ROOT})")
endif ()
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
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


add_subdirectory(src)
