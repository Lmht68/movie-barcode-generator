add_library(app_utils OBJECT)

target_sources(app_utils PRIVATE 
    # Headers
    constants.h
    logger.h

    # Sources
    logger.cpp
)

target_include_directories(app_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app_utils PUBLIC spdlog::spdlog)