add_subdirectory(utils)
add_subdirectory(gui)

add_executable(${PROJECT_NAME} 
    main.cpp
    ${CMAKE_SOURCE_DIR}/res/res.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    app_utils
    app_gui
    Qt6::Widgets 
    ${OpenCV_LIBS}
)

# Set UI properties for Windows
set_target_properties(${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)