cmake_minimum_required(VERSION 3.16)

project(VideoQualityMonitor VERSION 2.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the icon.
set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core QuickControls2 QmlWorkerScript)

include_directories("/opt/homebrew/Cellar/ffmpeg/6.1.1_3/include")
link_directories("/opt/homebrew/Cellar/ffmpeg/6.1.1_3/lib")

add_definitions(-DQT_NO_DEBUGS)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(VideoQualityMonitor
    main.cpp
    backend.h
    backend.cpp
    icon.icns
)

qt_add_qml_module(VideoQualityMonitor
    URI VideoQuality
    VERSION 2.0
    QML_FILES Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(VideoQualityMonitor PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appVideoQuality
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(VideoQualityMonitor PRIVATE
    Qt6::Quick
    Qt6::Core
    Qt6::Qml
    Qt6::QmlWorkerScript
    Qt6::QuickControls2
    libavcodec.a
    libavdevice.a
    libavfilter.a
    libavformat.a
    libavutil.a
    libpostproc.a
    libswresample.a
    libswscale.a
    libavcodec.60.dylib
)

include(GNUInstallDirs)
install(TARGETS VideoQualityMonitor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
