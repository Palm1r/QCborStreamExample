cmake_minimum_required(VERSION 3.17)

project(NetworkTestProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Network REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    qml.qrc
    src/device.h
    src/device.cpp
    src/rootController.h
    src/rootController.cpp
    src/devicesHub.h
    src/devicesHub.cpp
    src/deviceModel.h
    src/deviceModel.cpp
    )

add_executable(NetworkTestProject
    ${PROJECT_SOURCES}
    )

target_compile_definitions(NetworkTestProject
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(NetworkTestProject
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network)

