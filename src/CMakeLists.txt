
add_subdirectory(Core)
add_subdirectory(Scene)
add_subdirectory(Util)

add_executable(main
        main.cpp
        log.cpp
        log.h)

target_link_libraries(main
        ${OSG_LIBRARIES}
        ${Boost_LIBRARIES}
        ${PUGIXML_LIBRARY}
        ${CERES_LIBRARIES}
        ${OpenCV_LIBS}
        ${PROJECT_NAME}_Core
        ${PROJECT_NAME}_Util
        ${PROJECT_NAME}_Scene
        )
