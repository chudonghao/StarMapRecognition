set(MODEL_NAME ${PROJECT_NAME}_Scene)

add_library(${MODEL_NAME} STATIC
        MasterCameraManipulator.cpp
        MasterCameraManipulator.h
        MasterView.cpp
        MasterView.h
        Planet.cpp
        Planet.h
        SensorCameraManipulator.cpp
        SensorCameraManipulator.h
        SensorView.cpp
        SensorView.h
        Shape.cpp
        Shape.h
        StarGroup.cpp
        StarGroup.h
        CameraFrameNode.cpp
        CameraFrameNode.h)
target_link_libraries(${MODEL_NAME} ${OSG_LIBRARIES} ${PROJECT_NAME}_Core)
