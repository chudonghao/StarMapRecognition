
set(MODEL_NAME ${PROJECT_NAME}_Core)

add_library(${MODEL_NAME} STATIC
        CameraStarFunc.cpp
        CameraStarFunc.h
        Descriptor.cpp
        Descriptor.h
        StarGraph.cpp
        StarGraph.h
        StarTable.cpp
        StarTable.h
        StarPos.h
        StarPos.cpp
        Star.cpp
        Star.h
        StarOnGraph.cpp
        StarOnGraph.h
        StarOnSkySphere.cpp
        StarOnSkySphere.h
        SpecialCenterStarOnSkySphereGroup.cpp
        SpecialCenterStarOnSkySphereGroup.h
        Descriptor2.cpp
        Descriptor2.h
        DescriptorConverter.cpp
        DescriptorConverter.h
        SpecialCenterStarOnGraphGroup.cpp
        SpecialCenterStarOnGraphGroup.h)

target_link_libraries(${MODEL_NAME} ${PUGIXML_LIBRARY} ${OpenCV_LIBS} ${CERES_LIBRARIES})
