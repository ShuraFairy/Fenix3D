cmake_minimum_required(VERSION 3.16)
project(Fenix3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS OpenGL)
find_package(Qt6 COMPONENTS OpenGLWidgets)

if (WIN32 OR WIN64)
    set(INCLUDE_DIR "${INSTALL_EXAMPLESDIR}/include")
    include_directories(${INCLUDE_DIR})
    set(LINK_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/assimp/lib/Debug/assimp-vc142-mtd.lib)
endif ()

set(PROJECT_SOURCES
    glwidget.cpp
    glwidget.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    window.cpp
    window.h
    geometryengine.h
    geometryengine.cpp
    background.h
    background.cpp
    data.h
    transformational.h
    object3d.h
    object3d.cpp
    camera3d.cpp
    camera3d.h
    loader.h
    loader.cpp
    axis.h
    axis.cpp
)

qt6_add_resources(PROJECT_SOURCES shaders.qrc)

qt_add_executable(Fenix3D
    ${PROJECT_SOURCES}
)

set_target_properties(Fenix3D PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_link_libraries(Fenix3D PUBLIC
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Widgets
    ${LINK_LIBS}
)

target_include_directories(Fenix3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include)
