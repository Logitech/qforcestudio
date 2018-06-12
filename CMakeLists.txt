cmake_minimum_required(VERSION 3.11.0)

project(QForceStudio)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc, uic, rcc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find all the required Qt modules
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_definitions(-DQT5)
add_definitions(-DUNICODE -D_UNICODE)

set(FORMS
    AboutBoxDialog.ui
    ForceWidgets/FWCondition.ui
    ForceWidgets/FWConstant.ui
    ForceWidgets/FWPeriodic.ui
    ForceWidgets/FWRamp.ui
    ForceWidgets/FWRumble.ui
    GameControllerInfoWidget.ui
    MainWindow.ui
    SelectGameControllerDialog.ui
    Widgets/CodeGistWidget.ui
    Widgets/ConditionWidget.ui
    Widgets/DirectionWidget.ui
    Widgets/DurationWidget.ui
    Widgets/EnvelopeWidget.ui
    Widgets/ForceButtonBox.ui
    Widgets/PeriodFrequencyWidget.ui
    Widgets/RampWidget.ui
    Widgets/SendCommandDialog.ui)

set(RESOURCES
    Resources.qrc)

set(HEADERS
    AboutBoxDialog.h
    FakeGameController.h
    GameController.h
    GameControllerInfoWidget.h
    MainWindow.h
    QForceStudio.h
    SelectGameControllerDialog.h
    ForceEffects/FECondition.h
    ForceEffects/FEConstant.h
    ForceEffects/FEDamper.h
    ForceEffects/FEFriction.h
    ForceEffects/FEInertia.h
    ForceEffects/FEPeriodic.h
    ForceEffects/FERamp.h
    ForceEffects/FERumble.h
    ForceEffects/FESpring.h
    ForceEffects/ForceCondition.h
    ForceEffects/ForceEffect.h
    ForceEffects/ForceEnvelope.h
    ForceGraphWidgets/DragHandle.h
    ForceGraphWidgets/FGWCondition.h
    ForceGraphWidgets/FGWConstant.h
    ForceGraphWidgets/FGWPeriodic.h
    ForceGraphWidgets/FGWRamp.h
    ForceWidgets/ForceWidget.h
    ForceWidgets/FWCondition.h
    ForceWidgets/FWConstant.h
    ForceWidgets/FWDamper.h
    ForceWidgets/FWFriction.h
    ForceWidgets/FWInertia.h
    ForceWidgets/FWPeriodic.h
    ForceWidgets/FWRamp.h
    ForceWidgets/FWRumble.h
    ForceWidgets/FWSpring.h
    Widgets/AngleWidget.h
    Widgets/CodeGistWidget.h
    Widgets/ConditionWidget.h
    Widgets/DirectionWidget.h
    Widgets/DurationSliderWidget.h
    Widgets/DurationWidget.h
    Widgets/EnvelopeWidget.h
    Widgets/ForceButtonBox.h
    Widgets/PeriodFrequencyWidget.h
    Widgets/RampWidget.h
    Widgets/SendCommandDialog.h)

set(SOURCES
    AboutBoxDialog.cpp
    FakeGameController.cpp
    GameController.cpp
    GameControllerInfoWidget.cpp
    Main.cpp
    MainWindow.cpp
    QForceStudio.cpp
    SelectGameControllerDialog.cpp
    ForceEffects/FECondition.cpp
    ForceEffects/FEConstant.cpp
    ForceEffects/FEDamper.cpp
    ForceEffects/FEFriction.cpp
    ForceEffects/FEInertia.cpp
    ForceEffects/FEPeriodic.cpp
    ForceEffects/FERamp.cpp
    ForceEffects/FERumble.cpp
    ForceEffects/FESpring.cpp
    ForceEffects/ForceCondition.cpp
    ForceEffects/ForceEffect.cpp
    ForceEffects/ForceEnvelope.cpp
    ForceGraphWidgets/DragHandle.cpp
    ForceGraphWidgets/FGWCondition.cpp
    ForceGraphWidgets/FGWConstant.cpp
    ForceGraphWidgets/FGWPeriodic.cpp
    ForceGraphWidgets/FGWRamp.cpp
    ForceWidgets/ForceWidget.cpp
    ForceWidgets/FWCondition.cpp
    ForceWidgets/FWConstant.cpp
    ForceWidgets/FWDamper.cpp
    ForceWidgets/FWFriction.cpp
    ForceWidgets/FWInertia.cpp
    ForceWidgets/FWPeriodic.cpp
    ForceWidgets/FWRamp.cpp
    ForceWidgets/FWRumble.cpp
    ForceWidgets/FWSpring.cpp
    Widgets/AngleWidget.cpp
    Widgets/CodeGistWidget.cpp
    Widgets/ConditionWidget.cpp
    Widgets/DirectionWidget.cpp
    Widgets/DurationSliderWidget.cpp
    Widgets/DurationWidget.cpp
    Widgets/EnvelopeWidget.cpp
    Widgets/ForceButtonBox.cpp
    Widgets/PeriodFrequencyWidget.cpp
    Widgets/RampWidget.cpp
    Widgets/SendCommandDialog.cpp)

# platform specific files
if(APPLE)
    list(APPEND HEADERS
        OSXInput/linux/input.h)
elseif(WIN32)
    list(APPEND HEADERS
        GameControllerDirectInput.h)
    list(APPEND SOURCES
        GameControllerDirectInput.cpp
        Windows/import_windows_platform.cpp)
else()
    # TODO: add generic gamecontroller here
endif()

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE QForceStudio.icns)
    set_source_files_properties(
        QForceStudio.icns
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
    add_executable(QForceStudio MACOSX_BUNDLE
        ${SOURCES}
        ${HEADERS}
        ${UI_HEADERS}
        ${UI_RESOURCES})
    target_include_directories(QForceStudio PRIVATE
        OSXInput)
elseif(WIN32)
    list(APPEND SOURCES
        Windows/*.cpp)
    ##file(GLOB PLATFORM_SOURCES "Windows/*.cpp")

    # Change MS CRT to be static
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)

    # add winsock, shlwapi and opengl32 and the Qt EGL Libs
    set(PLATFORM_LIBRARIES "ws2_32" "ShlWAPI" "Opengl32" "Imm32" ${Qt5Gui_OPENGL_LIBRARIES} ${Qt5Gui_EGL_LIBRARIES})
    list(APPEND PLATFORM_LIBRARIES "d3d9")
    list(APPEND PLATFORM_LIBRARIES "dxguid")
    list(APPEND PLATFORM_LIBRARIES "winmm")

    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QtCore_libpath ${QtCore_location} DIRECTORY)
    list(APPEND PLATFORM_LIBRARIES "${Qt5Gui_PLUGINS}")

    add_executable(QForceStudio WIN32
        ${SOURCES}
        ${HEADERS}
        ${UI_HEADERS}
        ${UI_RESOURCES})
    add_executable(QForceStudio WIN32 ${SOURCES} ${HEADERS} ${BFORMS} ${UI_HEADERS} ${UI_RESOURCES} ${PLATFORM_SOURCES})
    target_link_libraries(QForceStudio debug "${QtCore_libpath}/preprocessord.lib")
    target_link_libraries(QForceStudio debug "${QtCore_libpath}/translatord.lib")
    target_link_libraries(QForceStudio debug "${QtCore_libpath}/Qt5PlatformSupportd.lib")
    target_link_libraries(QForceStudio optimized "${QtCore_libpath}/preprocessor.lib")
    target_link_libraries(QForceStudio optimized "${QtCore_libpath}/translator.lib")
    target_link_libraries(QForceStudio optimized "${QtCore_libpath}/Qt5PlatformSupport.lib")

else()
    add_executable(QForceStudio
        ${SOURCES}
        ${HEADERS}
        ${UI_HEADERS}
        ${UI_RESOURCES})
endif()

target_compile_features(QForceStudio PRIVATE
    cxx_std_11)

target_include_directories(QForceStudio PRIVATE
    ForceEffects
    ForceWidgets
    Widgets
    ForceGraphWidgets)

target_link_libraries(QForceStudio
    Qt5::Core
    Qt5::Widgets
    ${PLATFORM_LIBRARIES})
