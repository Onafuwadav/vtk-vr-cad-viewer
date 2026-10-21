
 
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
 
# Include CPack for packaging and installer creation
include(InstallRequiredSystemLibraries)
include(CPack)
 
# Set up packaging options
set(CPACK_PACKAGE_NAME "EEEE2076 - Software Engineering & VR Project")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "GROUP13")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This project implements a 3D CAD viewer using Qt and VTK, aimed at visualizing and manipulating 3D models. It provides basic functionalities such as loading STL files, applying rotation, and using filters like clipping and shrinking on the models. The user can interact with the models through a GUI interface, and advanced features such as VR support have been integrated to demonstrate a more immersive experience.")
set(CPACK_PACKAGE_EXECUTABLES "Software Engineering & VR Project")
 
# Set up the NSIS generator (if you want to use NSIS for your installer)
set(CPACK_GENERATOR "NSIS")
 
# Define the output directory for the installer
set(CPACK_NSIS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/installer")
 
# Include the installation of the necessary files
install(TARGETS FirstQCProject DESTINATION bin)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/Level1
        DESTINATION CADData
        FILES_MATCHING PATTERN "*")

# This will define a default executable to be bundled in the installer
install(FILES "${CMAKE_BINARY_DIR}/Debug/FirstQCProject.exe" DESTINATION bin)