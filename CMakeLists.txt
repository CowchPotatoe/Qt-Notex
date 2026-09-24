cmake_minimum_required(VERSION 3.16)

project(Notex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

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

# md4qt is header-only.
# The md4qt headers are located in thirdparty/md4qt/md4qt/.
target_include_directories(Notex PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/md4qt
)

# Enable md4qt's Qt support.
target_compile_definitions(Notex PRIVATE
    MD4QT_QT_SUPPORT
)

target_link_libraries(Notex PRIVATE
    Qt6::Widgets
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