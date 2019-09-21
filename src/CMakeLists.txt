
add_executable(main
        main.cpp
        CameraManipulator.cpp
        CameraManipulator.h
        Shape.cpp
        Shape.h
        CommonFunc.cpp
        CommonFunc.h
        CameraFrameNode.cpp
        CameraFrameNode.h
        SecondCameraManipulator.cpp
        SecondCameraManipulator.h
        Planet.cpp
        Planet.h
        )

target_link_libraries(main ${OSG_LIBRARIES} ${Boost_LIBRARIES} ${PUGIXML_LIBRARY})
