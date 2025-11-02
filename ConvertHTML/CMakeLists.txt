cmake_minimum_required(VERSION 3.16)

project(ConvertHTML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_ROOT "D:/QT/6.9.3/msvc2022_64")
set(CMAKE_PREFIX_PATH "${QT_ROOT}")

message(STATUS "🔍 Поиск Qt в: ${QT_ROOT}")

set(WEBENGINE_REQUIRED_COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
    Network
    WebChannel
    Positioning
    WebEngineCore
    WebEngineWidgets
)

set(ALL_COMPONENTS_FOUND TRUE)
set(MISSING_COMPONENTS "")

if(ALL_COMPONENTS_FOUND)
    find_package(Qt6 REQUIRED COMPONENTS ${WEBENGINE_REQUIRED_COMPONENTS})
    set(WEBENGINE_AVAILABLE TRUE)
else()
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)
    set(WEBENGINE_AVAILABLE FALSE)
endif()

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    converterbase.h
    basicconverter.h
    basicconverter.cpp
)

if(WEBENGINE_AVAILABLE)
    list(APPEND PROJECT_SOURCES
        advancedconverter.h
        advancedconverter.cpp
    )
else()
    list(APPEND PROJECT_SOURCES
        enhancedconverter.h
        enhancedconverter.cpp
    )

endif()


qt6_add_executable(ConvertHTML
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)


if(WEBENGINE_AVAILABLE)
    target_compile_definitions(ConvertHTML PRIVATE WEBENGINE_AVAILABLE)
endif()

if(WEBENGINE_AVAILABLE)
    target_link_libraries(ConvertHTML PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Network
        Qt6::WebChannel
        Qt6::Positioning
        Qt6::WebEngineCore
        Qt6::WebEngineWidgets
    )
else()
    target_link_libraries(ConvertHTML PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::PrintSupport
    )
endif()

set_target_properties(ConvertHTML PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ConvertHTML
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(ConvertHTML)

