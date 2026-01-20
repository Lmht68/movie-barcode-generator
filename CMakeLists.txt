cmake_minimum_required(VERSION 3.16)
project(movie-barcode-generator VERSION 0.1)

# App related definitions
set(APP_TARGET movie-barcode)   # internal
set(APP_NAME "MovieBarcode")    # display

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt auto tools if using Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(spdlog CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui OpenGLWidgets)
find_package(OpenCV REQUIRED)

# Include source directory
add_subdirectory(src)

# Add app icon resource for Windows
if (WIN32)
    target_sources(${APP_TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/res/res.rc
        ${CMAKE_SOURCE_DIR}/res/style/light/lightstyle.qrc
        ${CMAKE_SOURCE_DIR}/res/style/dark/darkstyle.qrc
    )
endif()