cmake_minimum_required(VERSION 2.8)

project(BeGameLauncher)

include_directories(headers headers/haiku_inner)
set(SOURCES
	src/BeApp.cpp
	src/BeMainWindow.cpp
	src/BeAboutWindow.cpp
	src/BeDirectoryFilter.cpp
	src/BeDirectoryFilePanel.cpp
	src/BeSettings.cpp
	src/BeUtils.cpp
	src/BeLauncherBase.cpp
	src/haiku_inner/HyperTextView.cpp
	src/haiku_inner/HyperTextActions.cpp
	BasedGameLauncher.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} be tracker localestub)
