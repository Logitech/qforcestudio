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
add_definitions(-DUNICODE -D_UNICODE)

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

# This is for customization of the OSX build - none of this works properly yet, it's
# just copied from a sample file. At some point, I should just drop OSX support completely...
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_executable(QForceStudio ${SOURCES} ${HEADERS} ${UI_HEADERS} ${UI_RESOURCES})
    set(MACOSX_BUNDLE_ICON_FILE QForceStudio.icns)
    SET_SOURCE_FILES_PROPERTIES(
      QForceStudio.icns
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
    )
    INCLUDE_DIRECTORIES("OSXInput")

elseif(WIN32)

  file(GLOB PLATFORM_SOURCES "Windows/*.cpp")

  # Change MS CRT to be static
  foreach(flag_var
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)

  # add winsock, shlwapi and opengl32 and the Qt EGL Libs
  set(PLATFORM_LIBRARIES "ws2_32" "ShlWAPI" "Opengl32" "Imm32" ${Qt5Gui_OPENGL_LIBRARIES} ${Qt5Gui_EGL_LIBRARIES})
  list(APPEND PLATFORM_LIBRARIES "d3d9")
  list(APPEND PLATFORM_LIBRARIES "dxguid")
  list(APPEND PLATFORM_LIBRARIES "winmm")

  # add the 3 missing stooges (Why Windows, oh Why?)
  get_target_property(QtCore_location Qt5::Core LOCATION)
  get_filename_component(QtCore_libpath ${QtCore_location} DIRECTORY)
  list(APPEND PLATFORM_LIBRARIES "${Qt5Gui_PLUGINS}")

  add_executable(QForceStudio WIN32 ${SOURCES} ${HEADERS} ${BFORMS} ${UI_HEADERS} ${UI_RESOURCES} ${PLATFORM_SOURCES})
  target_link_libraries(QForceStudio debug "${QtCore_libpath}/preprocessord.lib")
  target_link_libraries(QForceStudio debug "${QtCore_libpath}/translatord.lib")
  target_link_libraries(QForceStudio debug "${QtCore_libpath}/Qt5PlatformSupportd.lib")
  target_link_libraries(QForceStudio optimized "${QtCore_libpath}/preprocessor.lib")
  target_link_libraries(QForceStudio optimized "${QtCore_libpath}/translator.lib")
  target_link_libraries(QForceStudio optimized "${QtCore_libpath}/Qt5PlatformSupport.lib")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#qt5_use_modules(QForceStudio Widgets)
#target_link_libraries(QForceStudio ${Qt5Core_QTMAIN_LIBRARIES})

qt5_wrap_ui(UI_HEADERS ${BFORMS})
qt5_add_resources(UI_RESOURCES ${BRESOURCES})

qt5_use_modules(QForceStudio Widgets Concurrent)

target_link_libraries(QForceStudio ${Qt5Core_QTMAIN_LIBRARIES} ${PLATFORM_LIBRARIES})
