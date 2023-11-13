cmake_minimum_required (VERSION 3.15)
project (FlowUI VERSION 1.0.0 LANGUAGES CXX C)

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


set (FLOWUI_INC "${CMAKE_CURRENT_SOURCE_DIR}/srcs")

file (GLOB_RECURSE FLOWUI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/srcs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/srcs/*.h")
add_library (libFlowUI STATIC ${FLOWUI_SRC})
set_target_properties (libFlowUI PROPERTIES
  PREFIX "lib" IMPORT_PREFIX "lib" OUTPUT_NAME "FlowUI")
target_include_directories (libFlowUI PUBLIC ${FLOWUI_INC})
target_link_libraries (libFlowUI PUBLIC libJUCE)

if (MSVC)
    target_compile_definitions (libFlowUI PUBLIC "JUCE_DLL=1" "JUCE_DISABLE_DLL_ALLOCATORS=1")
endif (MSVC)

if (DEFINED FLOWUI_OUTPUT)
    set_target_properties (libFlowUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FLOWUI_OUTPUT}")
endif (DEFINED FLOWUI_OUTPUT)