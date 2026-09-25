cmake_minimum_required(VERSION 3.16)

# Prevent Qt Creator's injected CMake helper from running.
set(CMAKE_PROJECT_INCLUDE_BEFORE "")

project(Notex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    WebEngineWidgets
)

# --------------------------------------------------
# Notex
# --------------------------------------------------

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources/resources.qrc
    markdownparser.h
    markdownparser.cpp
    documents.h
    documents.cpp
)

qt_add_executable(Notex
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# --------------------------------------------------
# md4qt
# --------------------------------------------------

# md4qt is header-only.
# Its headers are located in thirdparty/md4qt/md4qt/.
target_include_directories(Notex PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/md4qt
)

# Enable md4qt's Qt support.
target_compile_definitions(Notex PRIVATE
    MD4QT_QT_SUPPORT
)

# --------------------------------------------------
# KaTeX
# --------------------------------------------------

file(GLOB KATEX_MAIN_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/katex/*.css
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/katex/*.js
)

file(GLOB KATEX_CONTRIB_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/katex/contrib/*.js
)

file(GLOB KATEX_FONT_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/katex/fonts/*
)

qt_add_resources(Notex
    "katex"
    PREFIX "/katex"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/katex"
    FILES
        ${KATEX_MAIN_FILES}
        ${KATEX_CONTRIB_FILES}
        ${KATEX_FONT_FILES}
)

# --------------------------------------------------
# Libraries
# --------------------------------------------------

target_link_libraries(Notex PRIVATE
    Qt6::Widgets
    Qt6::WebEngineWidgets
)

set_target_properties(Notex PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib64"
)

include(GNUInstallDirs)

install(TARGETS Notex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(Notex)