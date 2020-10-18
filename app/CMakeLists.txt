cmake_minimum_required(VERSION 3.14)

project(app LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

if(ANDROID)
    add_library(app SHARED
      main.cpp
      qml.qrc
    )
else()
    add_executable(app
      main.cpp
      qml.qrc
      firebaseconfig.cpp
      firebaseconfig.h
    )
endif()

target_compile_features(app PRIVATE cxx_std_17)

target_compile_definitions(app
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(app
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick qt-firebase-sdk)
