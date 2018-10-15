cmake_minimum_required(VERSION 2.8)

project(BeGameLauncher)

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

add_executable(BasedGameLauncher BasedGameLauncher.cpp ${SOURCES})
target_link_libraries(BasedGameLauncher ${LIBRARIES})
