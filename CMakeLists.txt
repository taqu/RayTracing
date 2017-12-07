cmake_minimum_required(VERSION 3.7)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(ProjectName RayTracing)
project(${ProjectName})

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/common ${CMAKE_CURRENT_SOURCE_DIR}/test)


if(MSVC)
    set(DEFAULT_CXX_FLAGS "/DWIN32 /D_WINDOWS /D_MBCS /W4 /WX- /nologo /fp:precise /Zc:wchar_t /TP /Gd")
    if("1800" VERSION_LESS MSVC_VERSION)
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /EHsc")
    endif()

    set(DEFAULT_CXX_FLAGS_DEBUG "/D_DEBUG /DMU3_EXPORTS /MTd /Zi /Ob0 /Od /RTC1 /Gy /GR- /GS /Gm-")
    set(DEFAULT_CXX_FLAGS_RELEASE "/DMU3_EXPORTS /MT /O2 /GL /GR- /DNDEBUG")

elseif(UNIX)
    set(DEFAULT_CXX_FLAGS "-Wall -O2 -std=c++11")
elseif(APPLE)
endif()

add_subdirectory(tutorial00)

