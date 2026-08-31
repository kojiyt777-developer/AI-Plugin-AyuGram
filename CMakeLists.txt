cmake_minimum_required(VERSION 3.16)
project(AiPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Network)

add_library(AiPlugin SHARED ai_plugin.cpp)

target_link_libraries(AiPlugin Qt5::Core Qt5::Network)

set_target_properties(AiPlugin PROPERTIES
    PREFIX ""
    SUFFIX ".plugin"
)
