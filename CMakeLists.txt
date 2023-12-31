﻿cmake_minimum_required (VERSION 3.15)
project (flowui VERSION 1.0.0 LANGUAGES CXX C)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif (NOT DEFINED CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 20)#使用C++20
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 17)#使用C17
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_C_STANDARD_REQUIRED ON)

set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")#MSVC下使用/MD和/MDd

find_package (juce-host-dev-kit CONFIG REQUIRED)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set (FLOWUI_INSTALL_NAME ${PROJECT_NAME})

set (FLOWUI_INC "${CMAKE_CURRENT_SOURCE_DIR}/srcs")

# =======================================
# Target
# =======================================
file (GLOB_RECURSE FLOWUI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/srcs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/srcs/*.h")
add_library (flowui STATIC ${FLOWUI_SRC})
if (MSVC)
	target_link_libraries (flowui PUBLIC juce-host-dev-kit::juce-core)
else (MSVC)
	target_link_libraries (flowui PUBLIC juce-host-dev-kit::juce-core-static)#To avoid "[[deprecated]] __attribute__ ((visibility ("default")))" failure.
endif (MSVC)

foreach (ITEM ${FLOWUI_INC})
	file (RELATIVE_PATH ITEM_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${ITEM})
	target_include_directories(flowui PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${ITEM_PATH}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${FLOWUI_INSTALL_NAME}/${ITEM_PATH}>"
    )
endforeach (ITEM ${FLOWUI_INC})

# ============================
# Install
# ============================
set (FLOWUI_CMAKE_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${FLOWUI_INSTALL_NAME})

install (TARGETS flowui
	EXPORT ${FLOWUI_INSTALL_NAME}Targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" OPTIONAL
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
)

foreach (ITEM ${FLOWUI_INC})
	file (GLOB_RECURSE HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${ITEM}/*.h")

	foreach (HDR_FILE ${HDR_FILES})
		get_filename_component (HDR_DIR ${HDR_FILE} DIRECTORY)
		install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/${HDR_FILE}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${FLOWUI_INSTALL_NAME}/${HDR_DIR}" OPTIONAL)
	endforeach (HDR_FILE ${HDR_FILES})
endforeach (ITEM ${FLOWUI_INC})

write_basic_package_version_file (
	"${CMAKE_CURRENT_BINARY_DIR}/${FLOWUI_INSTALL_NAME}-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/${FLOWUI_INSTALL_NAME}-config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${FLOWUI_INSTALL_NAME}-config.cmake"
	INSTALL_DESTINATION ${FLOWUI_CMAKE_CONFIG_INSTALL_DIR}
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install (FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${FLOWUI_INSTALL_NAME}-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${FLOWUI_INSTALL_NAME}-config-version.cmake"
	DESTINATION ${FLOWUI_CMAKE_CONFIG_INSTALL_DIR}
)

install (EXPORT ${FLOWUI_INSTALL_NAME}Targets
	FILE "${FLOWUI_INSTALL_NAME}-targets.cmake"
	NAMESPACE ${FLOWUI_INSTALL_NAME}::
	DESTINATION ${FLOWUI_CMAKE_CONFIG_INSTALL_DIR}
)