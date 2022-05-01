cmake_minimum_required(VERSION 3.5)

project(gradient_dialog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(gradient_dialog STATIC
  gradient_dialog.cc
  gradient_dialog.h
  gradient_dialog.ui
  gradient_edit.cc
  gradient_edit.h
  gradient_stops_edit.cc
  gradient_stops_edit.h
  gradient_variant.cc
  gradient_variant.h
)

target_link_libraries(gradient_dialog PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(gradient_dialog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
