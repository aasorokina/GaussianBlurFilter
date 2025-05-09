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

file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE SRC_H   CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE QRC_FILES CONFIGURE_DEPENDS resources/*.qrc)

set(PROJECT_SOURCES
    ${SRC_CPP}
    ${SRC_H}
    ${QRC_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gaussian_filter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(gaussian_filter
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(gaussian_filter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Concurrent)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gaussian_filter)
endif()
