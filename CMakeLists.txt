cmake_minimum_required(VERSION 2.8)

project(BeGameLauncher)
include_directories(headers)

set(SOURCES src/BeApp.cpp
	src/BeMainWindow.cpp
	BasedGameLauncher.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} be)
