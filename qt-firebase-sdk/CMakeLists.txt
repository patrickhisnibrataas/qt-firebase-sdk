cmake_minimum_required(VERSION 3.14)

project(qt-firebase-sdk LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(firebase_app)
find_package(firebase_auth)

add_library(${PROJECT_NAME} STATIC
  firebaseapp.cpp
  firebaseapp.h
  firebaseauth.cpp
  firebaseauth.h
  firebaseuser.cpp
  firebaseuser.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
    PUBLIC
        firebase_app firebase_auth)

target_compile_definitions(${PROJECT_NAME} PRIVATE QTFIREBASESDK_LIBRARY)
