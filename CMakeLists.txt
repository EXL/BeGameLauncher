cmake_minimum_required(VERSION 2.8)

project(BeGameLauncher CXX)

if(NOT HAIKU)
	message(FATAL_ERROR "This application only supports the Haiku OS!")
endif()

include_directories(headers)

set(SOURCES
	src/BeApp.cpp
	src/BeMainWindow.cpp
	src/BeAboutWindow.cpp
	src/BeDirectoryFilter.cpp
	src/BeDirectoryFilePanel.cpp
	src/BeSettings.cpp
	src/BeUtils.cpp
	src/BeLauncherBase.cpp
	src/BeUrlStringView.cpp
	src/BeMultiStringView.cpp
	src/BeImageView.cpp)

set(LIBRARIES
	be
	tracker
	translation
	localestub)

set(BASED_LAUNCHER_NAME "BasedGameLauncher")
add_executable(${BASED_LAUNCHER_NAME} ${BASED_LAUNCHER_NAME}.cpp ${SOURCES})
target_link_libraries(${BASED_LAUNCHER_NAME} ${LIBRARIES})
add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	rc -o ${BASED_LAUNCHER_NAME}.rsrc ${CMAKE_CURRENT_SOURCE_DIR}/res/based/${BASED_LAUNCHER_NAME}.rdef)
add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	xres -o ${BASED_LAUNCHER_NAME} ${BASED_LAUNCHER_NAME}.rsrc)
add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	mimeset --all ${BASED_LAUNCHER_NAME})
