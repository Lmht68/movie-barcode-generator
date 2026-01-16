add_library(app_gui STATIC
    # Headers
    main_window.h
    central_widget.h
    barcode_view.h
    file_browser_dock.h
    file_browser_model.h
    media_filter_proxy.h

    # Sources
    main_window.cpp
    central_widget.cpp
    barcode_view.cpp
    file_browser_dock.cpp
    file_browser_model.cpp
    media_filter_proxy.cpp
)

target_include_directories(app_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app_gui PUBLIC 
    app_utils
    Qt::Widgets
)

# App related definitions
target_compile_definitions(app_gui PRIVATE WINDOW_TITLE="${APP_NAME} v${PROJECT_VERSION}")