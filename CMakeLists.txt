cmake_minimum_required(VERSION 2.8.12)

if(NOT EXISTS $ENV{QTDIR})
    message(FATAL_ERROR "Please define an environment variable QTDIR and point it to your Qt installation directory.")
endif()

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
message("Using Qt at path: " ${CMAKE_PREFIX_PATH})

project(QForceStudio)

# Build a debug version
set(CMAKE_BUILD_TYPE Debug)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find all the required Qt modules
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT5)

file(GLOB FORMS     *.ui)
file(GLOB RESOURCES *.qrc)
file(GLOB HEADERS   *.h)
file(GLOB SOURCES   *.cpp)

foreach(SUBDIR ForceEffects ForceWidgets Widgets ForceGraphWidgets)
    include_directories(${SUBDIR})
    add_subdirectory(${SUBDIR})
endforeach(SUBDIR)

qt5_wrap_ui(UI_HEADERS ${FORMS})
qt5_add_resources(UI_RESOURCES ${RESOURCES})
add_executable(QForceStudio ${SOURCES} ${HEADERS} ${UI_HEADERS} ${UI_RESOURCES})

# This is for customization of the OSX build - none of this works properly yet, it's
# just copied from a sample file. At some point, I should just drop OSX support completely...
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX_BUNDLE_ICON_FILE QForceStudio.icns)
    SET_SOURCE_FILES_PROPERTIES(
      QForceStudio.icns
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
    )
    INCLUDE_DIRECTORIES("OSXInput")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

qt5_use_modules(QForceStudio Widgets)

target_link_libraries(QForceStudio ${Qt5Core_QTMAIN_LIBRARIES})
