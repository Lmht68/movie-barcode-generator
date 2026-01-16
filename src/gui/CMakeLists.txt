add_library(app_gui STATIC
    # Headers
    main_window.h
    widget/central_widget.h
    widget/file_browser_dock.h
    view/barcode_view.h
    model/file_browser_model.h
    model/media_filter_proxy.h

    # Sources
    main_window.cpp
    widget/central_widget.cpp
    widget/file_browser_dock.cpp
    view/barcode_view.cpp
    model/file_browser_model.cpp
    model/media_filter_proxy.cpp
)

target_include_directories(app_gui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(app_gui PUBLIC 
    app_utils
    Qt::Widgets
)

# App related definitions
target_compile_definitions(app_gui PRIVATE WINDOW_TITLE="${APP_NAME} v${PROJECT_VERSION}")