cmake_minimum_required(VERSION 3.7)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(ProjectName RayTracing)
project(${ProjectName})

function(gather_files FILES SOURCE)
    file (GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE})
    set(${FILES} ${SOURCES} PARENT_SCOPE)
endfunction(gather_files)

function(gather_lib_files COMMON_HEADERS COMMON_SOURCES ROOT LIBNAME MODULES)
    set(ROOT_INCLUDE "${ROOT}/${LIBNAME}/")
    set(ROOT_SOURCE "${ROOT}/${LIBNAME}/src")

    set(CMN_HEADERS "")
    set(CMN_SOURCES "")

    foreach(MODULE IN LISTS MODULES)
        set(HFILES "")
        set(CPPFILES "")
        gather_files(HFILES "${ROOT_INCLUDE}${MODULE}/*.h")
        gather_files(CPPFILES "${ROOT_SOURCE}${MODULE}/*.cpp")

        # Group files
        string(REPLACE "/" "\\" FILTER "include/${LIBNAME}/${MODULE}")
        source_group("${FILTER}" FILES ${HFILES})
        string(REPLACE "/" "\\" FILTER "src/${LIBNAME}/${MODULE}")
        source_group("${FILTER}" FILES ${CPPFILES})
        set(CMN_HEADERS ${CMN_HEADERS} ${HFILES})
        set(CMN_SOURCES ${CMN_SOURCES} ${CPPFILES})
    endforeach()
    set(${COMMON_HEADERS} ${CMN_HEADERS} PARENT_SCOPE)
    set(${COMMON_SOURCES} ${CMN_SOURCES} PARENT_SCOPE)
endfunction(gather_lib_files)

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/common ${CMAKE_CURRENT_SOURCE_DIR}/test)
include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/lray)

if(MSVC)
    set(DEFAULT_CXX_FLAGS "/DWIN32 /D_WINDOWS /D_MBCS /W4 /WX- /nologo /fp:precise /arch:AVX /Zc:wchar_t /TP /Gd")
    if("1800" VERSION_LESS MSVC_VERSION)
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /EHsc")
    endif()

    set(DEFAULT_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 /Gy /GR- /GS /Gm-")
    set(DEFAULT_CXX_FLAGS_RELEASE "/MT /O2 /GL /GR- /DNDEBUG")
    set(DEFAULT_CXX_LINK_FLAGS_DEBUG "")
    set(DEFAULT_CXX_LINK_FLAGS_RELEASE "/LTCG")

elseif(UNIX)
    set(DEFAULT_CXX_FLAGS "-Wall -O2 -std=c++11 -std=gnu++11 -march=native")
elseif(APPLE)
endif()

add_subdirectory(test)
add_subdirectory(tutorial00)
add_subdirectory(tutorial01)
add_subdirectory(tutorial02)
add_subdirectory(tutorial03)
