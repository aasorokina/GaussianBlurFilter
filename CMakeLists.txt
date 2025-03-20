cmake_minimum_required(VERSION 3.5)

project(gaussian_filter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Concurrent REQUIRED)

set(PROJECT_SOURCES
        include/mainwindow.h
        include/sourceimgwidget.h
        include/filterimagewidget.h
        include/gaussianfilter.h
        src/main.cpp
        src/mainwindow.cpp
        src/sourceimgwidget.cpp
        src/filterimagewidget.cpp
        src/gaussianfilter.cpp
        resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gaussian_filter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(gaussian_filter SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(gaussian_filter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(gaussian_filter Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Concurrent)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gaussian_filter)
endif()
