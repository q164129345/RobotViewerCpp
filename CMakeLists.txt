cmake_minimum_required(VERSION 3.16)

project(RobotViewerCpp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Mqtt)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appRobotViewerCpp
    main.cpp
)

qt_add_qml_module(appRobotViewerCpp
    URI RobotViewerCpp
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES component/common.h component/cr_protocol.cpp component/cr_protocol.h component/hello_world.cpp component/hello_world.h component/mqttclient.cpp component/mqttclient.h component/robot_message.cpp component/robot_message.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appRobotViewerCpp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRobotViewerCpp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRobotViewerCpp
    PRIVATE Qt6::Quick Qt6::Mqtt
)

include(GNUInstallDirs)
install(TARGETS appRobotViewerCpp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
