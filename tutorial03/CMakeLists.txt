cmake_minimum_required(VERSION 3.7)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(ProjectName Tutorial03)
project(${ProjectName})

set(HEADERS "")
set(SOURCES "main.cpp")

set(COMMON_HEADERS "")
set(COMMON_SOURCES "")
set(MODULES "/" "/core" "/math" "/shape" "/scene" "/accel")
gather_lib_files(COMMON_HEADERS COMMON_SOURCES ".." lray "${MODULES}")

source_group("include" FILES ${HEADERS})
source_group("src" FILES ${SOURCES})

set(FILES ${SOURCES} ${COMMON_SOURCES} ${COMMON_HEADERS})

set(OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

add_executable(${ProjectName} ${FILES})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${DEFAULT_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${DEFAULT_CXX_FLAGS_RELEASE}")
    target_link_libraries(${ProjectName} "winmm.lib")
    set_target_properties(${ProjectName} PROPERTIES
        LINK_FLAGS_DEBUG "${DEFAULT_CXX_LINK_FLAGS_DEBUG}"
        LINK_FLAGS_RELEASE "${DEFAULT_CXX_LINK_FLAGS_RELEASE}")

elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
elseif(APPLE)
endif()

set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG "${ProjectName}" OUTPUT_NAME_RELEASE "${ProjectName}")
