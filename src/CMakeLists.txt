add_subdirectory(utils)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    app_utils
    Qt6::Widgets 
    ${OpenCV_LIBS}
)

# Set UI properties for Windows
set_target_properties(${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)