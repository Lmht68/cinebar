add_subdirectory(utils)
add_subdirectory(gui)

add_executable(${APP_TARGET} 
    main.cpp
    ${CMAKE_SOURCE_DIR}/res/res.qrc
)

target_link_libraries(${APP_TARGET} PRIVATE 
    app_utils
    app_gui
    Qt6::Widgets 
    ${OpenCV_LIBS}
)

# Set UI properties for Windows
set_target_properties(${APP_TARGET} PROPERTIES 
    OUTPUT_NAME "${APP_NAME}" 
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)