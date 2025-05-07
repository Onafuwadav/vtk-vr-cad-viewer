

cmake_minimum_required(VERSION 3.16)

project(FirstQCProject)


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ModelPart.cpp
        ModelPart.h
        ModelPartList.cpp
        ModelPartList.h
        icons.qrc
	VRRenderThread.cpp
	VRRenderThread.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FirstQCProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        optiondialog.h optiondialog.cpp optiondialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FirstQCProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FirstQCProject SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FirstQCProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FirstQCProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})


include(GNUInstallDirs)
install(TARGETS FirstQCProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FirstQCProject)
endif()
# Copy across Open VR bindings that map controllers
# The program will expect to find these in the build dir when it runs
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/
)
