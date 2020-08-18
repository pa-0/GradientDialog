cmake_minimum_required(VERSION 3.5)

project(gradient_dialog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

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

target_link_libraries(gradient_dialog PRIVATE Qt5::Widgets)

target_compile_definitions(gradient_dialog PRIVATE GRADIENT_DIALOG_LIBRARY)
target_include_directories(gradient_dialog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
