cmake_minimum_required(VERSION 3.1)

project(BeGameLauncher CXX)

if(NOT HAIKU)
	message(FATAL_ERROR "This application only supports the Haiku OS!")
endif()

set(CMAKE_CXX_STANDARD 98)
#set(CMAKE_VERBOSE_MAKEFILE ON)
#add_compile_options(-Wall -Wextra -pedantic -Werror)

include_directories(headers)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeApp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeMainWindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeAboutWindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeAboutView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeAboutStripeView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeDirectoryFilter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeDirectoryFilePanel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeSettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeLauncherBase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeLauncherView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeUrlStringView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeMultiStringView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeUnderlineStringView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/BeImageView.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

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
add_executable(${BASED_LAUNCHER_NAME} ${BASED_LAUNCHER_NAME}.cpp)
target_link_libraries(${BASED_LAUNCHER_NAME} ${PROJECT_NAME} ${LIBRARIES})

set(P "------")
add_custom_target(Resources ALL
	DEPENDS ${BASED_LAUNCHER_NAME}
	SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/res/${RES_DIR}/${BASED_LAUNCHER_NAME}.rdef)
add_custom_command(TARGET Resources
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/res/${RES_DIR}
	VERBATIM)
add_custom_command(TARGET Resources
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/pre
	VERBATIM)
add_custom_command(TARGET Resources
	COMMAND rc -o ${BASED_LAUNCHER_NAME}.rsrc
		${CMAKE_CURRENT_SOURCE_DIR}/res/${RES_DIR}/${BASED_LAUNCHER_NAME}.rdef
	COMMENT "${P} Compiling '${BASED_LAUNCHER_NAME}.rsrc' resource file from '${BASED_LAUNCHER_NAME}.rdef' script."
	VERBATIM)
add_custom_command(TARGET Resources
	COMMAND xres -o ${BASED_LAUNCHER_NAME} ${BASED_LAUNCHER_NAME}.rsrc
	COMMENT "${P} Adding compiled '${BASED_LAUNCHER_NAME}.rsrc' resources to '${BASED_LAUNCHER_NAME}' executable."
	VERBATIM)
add_custom_command(TARGET Resources
	COMMAND mimeset --all ${BASED_LAUNCHER_NAME}
	COMMENT "${P} Updating MIME types for '${BASED_LAUNCHER_NAME}' executable."
	VERBATIM)
add_custom_command(TARGET Resources
	COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/${BASED_LAUNCHER_NAME}.cpp ${SOURCES} |
		${CMAKE_CXX_COMPILER} -E -x c++ -I${CMAKE_CURRENT_SOURCE_DIR}/headers
		-DSIGNATURE="${SIGNATURE}" -DB_COLLECTING_CATKEYS - | grep -av "^#" >
		${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/pre/${BASED_LAUNCHER_NAME}.pre
	COMMENT "${P} Collecting translation strings in the source files to the '${BASED_LAUNCHER_NAME}.pre' file."
	VERBATIM)

# English locale must be linked first and the all rest last. Therefore, we build a chain of dependencies
# where locales are linked to the executable file in the order which specified in the ${LOCALES} list. This
# will help get rid of racing conditions in a parallel build and avoid some strange bugs.
list(LENGTH LOCALES LOCALES_SIZE)
math(EXPR LOCALES_SIZE_MAX "${LOCALES_SIZE} - 1")
foreach(VAL RANGE ${LOCALES_SIZE_MAX})
	list(GET LOCALES ${VAL} LOCALE)
	math(EXPR VAL_PREVIOUS "${VAL} - 1")
	list(GET LOCALES ${VAL_PREVIOUS} LOCALE_PREVIOUS)
	if(${VAL} EQUAL 0)
		set(TARGET_DEPENDS Resources)
	else()
		set(TARGET_DEPENDS Language-${LOCALE_PREVIOUS})
	endif()
	add_custom_target(Language-${LOCALE} ALL
		DEPENDS ${TARGET_DEPENDS}
		SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys)
	add_custom_command(TARGET Language-${LOCALE}
		COMMAND test -f ${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys ||
			collectcatkeys -l ${LOCALE} -s ${SIGNATURE}
			${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/pre/${BASED_LAUNCHER_NAME}.pre -o
			${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys
		COMMENT "${P} Generating translation cat-keys for '${LOCALE}' locale."
		VERBATIM)
	add_custom_command(TARGET Language-${LOCALE}
		COMMAND linkcatkeys -o ${BASED_LAUNCHER_NAME} -s ${SIGNATURE} -tr -l ${LOCALE}
			${CMAKE_CURRENT_SOURCE_DIR}/locales/${RES_DIR}/${LOCALE}.catkeys
		COMMENT "${P} Adding translation cat-keys for '${LOCALE}' to the executable file."
		VERBATIM)
endforeach()
