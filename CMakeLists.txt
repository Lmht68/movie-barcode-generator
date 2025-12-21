cmake_minimum_required(VERSION 3.16)
project(movie-barcode-gen VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt auto tools if using Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

add_subdirectory(src)