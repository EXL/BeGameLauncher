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
	src/BeAboutView.cpp
	src/BeAboutStripeView.cpp
	src/BeDirectoryFilter.cpp
	src/BeDirectoryFilePanel.cpp
	src/BeSettings.cpp
	src/BeUtils.cpp
	src/BeLauncherBase.cpp
	src/BeLauncherView.cpp
	src/BeUrlStringView.cpp
	src/BeMultiStringView.cpp
	src/BeUnderlineStringView.cpp
	src/BeImageView.cpp)

set(LIBRARIES
	be
	tracker
	translation
	localestub)

set(LOCALES
	en
	ru)

if(NOT LAUNCHER OR LAUNCHER STREQUAL "based")
	set(BASED_LAUNCHER_NAME "BasedGameLauncher")
	set(RES_DIR "based")
	set(SIGNATURE "application/x-vnd.exl-BasedGameLauncher")
endif()

add_definitions(-DSIGNATURE="${SIGNATURE}")
add_executable(${BASED_LAUNCHER_NAME} ${BASED_LAUNCHER_NAME}.cpp ${SOURCES})
target_link_libraries(${BASED_LAUNCHER_NAME} ${LIBRARIES})

add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	rc -o ${BASED_LAUNCHER_NAME}.rsrc ${CMAKE_CURRENT_SOURCE_DIR}/res/${RES_DIR}/${BASED_LAUNCHER_NAME}.rdef
	COMMENT "Compiling resources.")
add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	xres -o ${BASED_LAUNCHER_NAME} ${BASED_LAUNCHER_NAME}.rsrc
	COMMENT "Adding compiled resources to the executable file.")
add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	mimeset --all ${BASED_LAUNCHER_NAME}
	COMMENT "Updating MIME types.")
add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
	cat ${CMAKE_CURRENT_SOURCE_DIR}/${BASED_LAUNCHER_NAME}.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp |
	${CMAKE_CXX_COMPILER} -E -x c++ -I${CMAKE_CURRENT_SOURCE_DIR}/headers -DSIGNATURE="${SIGNATURE}"
	-DB_COLLECTING_CATKEYS - | grep -av "^#" >
	${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/pre/${BASED_LAUNCHER_NAME}.pre
	COMMENT "Collecting translation strings in the source files.")
foreach(LOCALE ${LOCALES})
	add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
		test -f ${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys ||
		collectcatkeys -l ${LOCALE} -s ${SIGNATURE}
		${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/pre/${BASED_LAUNCHER_NAME}.pre -o
		${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys
		COMMENT "Generating translation cat-keys for '${LOCALE}' locale.")
	add_custom_command(TARGET ${BASED_LAUNCHER_NAME} COMMAND
		linkcatkeys -o ${BASED_LAUNCHER_NAME} -s ${SIGNATURE} -tr -l ${LOCALE}
		${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys
		COMMENT "Adding translation cat-keys for '${LOCALE}' to the executable file.")
endforeach()
