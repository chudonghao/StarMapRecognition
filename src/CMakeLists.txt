
add_executable(main
        main.cpp
        MasterCameraManipulator.cpp
        MasterCameraManipulator.h
        Shape.cpp
        Shape.h
        CommonFunc.cpp
        CommonFunc.h
        CameraFrameNode.cpp
        CameraFrameNode.h
        SensorCameraManipulator.cpp
        SensorCameraManipulator.h
        Planet.cpp
        Planet.h
        CameraStarFunc.cpp
        CameraStarFunc.h
        MasterView.cpp
        MasterView.h
        SensorView.cpp
        SensorView.h
        StarTable.cpp
        StarTable.h
        StarGroup.cpp
        StarGroup.h
        Descriptor.cpp
        Descriptor.h
        StarGraph.cpp
        StarGraph.h)

target_link_libraries(main ${OSG_LIBRARIES} ${Boost_LIBRARIES} ${PUGIXML_LIBRARY} ${CERES_LIBRARIES})
