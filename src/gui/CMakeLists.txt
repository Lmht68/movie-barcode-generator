add_library(app_gui STATIC
    # Headers
    mainwindow.h

    # Sources
    mainwindow.cpp
)

target_include_directories(app_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app_gui PUBLIC 
    app_utils
    Qt::Widgets
)

# App related definitions
set(APP_TITLE "Movie Barcode Generator")
target_compile_definitions(app_gui PRIVATE WINDOW_TITLE="${APP_TITLE} v${PROJECT_VERSION}")