##########################################################################
#                          Starlight Physics v0.0.1
#         Created by Ranyodh Singh Mandur - 🍑 2026-2026
#
#              Licensed under the MIT License (MIT).
#         For more details, see the LICENSE file or visit:
#               https://opensource.org/licenses/MIT
#
#       Starlight Physics is a free open source physics engine
##########################################################################

####################################### Config Options

option(BUILD_STARLIGHT_SHARED "Builds starlight_physics as a shared library" OFF) #build starlight_physics shared by default uwu
option(BUILD_STARLIGHT_TESTS "build test suite" ON)

####################################### Enforce CMake Version

cmake_minimum_required(VERSION 4.2.0) #need this for vs2026

####################################### Project definition

project(Starlight_Physics VERSION 0.0.1 DESCRIPTION "Starlight Physics" LANGUAGES C)

####################################### Enforce C17 Standard

set(CMAKE_C_STANDARD 17) #ye
set(CMAKE_C_STANDARD_REQUIRED ON)

####################################### Random Globals

set_property(GLOBAL PROPERTY USE_FOLDERS ON) #dunno where else to put this owo

# Add Compiler Warnings for Clang/GCC (assuming those are the only two ur using on unix like systems uwu)
if (PEACH_PLATFORM_IS_UNIX)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

####################################### Check For No Config Set

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "No build type specified, ending build. CMAKE WILL NOW EXIT")
endif()

####################################### Enforce 64-bit Requirement

if(NOT ${CMAKE_SIZEOF_VOID_P} EQUAL 8 AND NOT PEACH_VITA)
    message(FATAL_ERROR "Starlight Physics only supports 64-bit platforms (except PS Vita).")
endif()

####################################### Find starlight_physics source and headers

# Peach Core
file(
    GLOB_RECURSE 
    STARLIGHT_PHYSICS_SOURCES 
    CONFIGURE_DEPENDS 
    "src/*.c" 
    "src/*.h"
)

    source_group(           
        TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
        PREFIX "starlight_physics"
        FILES ${STARLIGHT_PHYSICS_SOURCES}
    )

####################################### Static/Dynamic Library Target

if(BUILD_STARLIGHT_SHARED)
    set(STARLIGHT_BUILD_TYPE SHARED)
else()
    set(STARLIGHT_BUILD_TYPE STATIC)
endif()

# Create a static or dynamic library for starlight_physics
add_library(starlight_physics ${STARLIGHT_BUILD_TYPE} 
    ${STARLIGHT_PHYSICS_SOURCES}
)

target_include_directories(starlight_physics PUBLIC
    ${PROJECT_SOURCE_DIR}/third_party/ #header only library includes
    ${PROJECT_SOURCE_DIR}/src

)

set_target_properties(starlight_physics PROPERTIES POSITION_INDEPENDENT_CODE ON) #make starlight_physics PIC friendly UwU

####################################### Master snuggle list for platform detection UwU (my chatgpt is bricked af)
# daddy guarantees exactly ONE of these is always defined per build so no ambiguity 🍑
# daddy guaranteed so ig it works OwO

if(PEACH_WINDOWS)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_WINDOWS
        PEACH_PLATFORM_DESKTOP
        PEACH_ARCH_X64
    )

elseif(PEACH_WINDOWS_ARM64)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_WINDOWS
        PEACH_PLATFORM_DESKTOP
        PEACH_ARCH_ARM64
    )

elseif(PEACH_MACOS)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_MACOS
        PEACH_PLATFORM_APPLE
        PEACH_PLATFORM_DESKTOP
        PEACH_ARCH_ARM64 # universal binary covers both but arm64 is primary
    )

elseif(PEACH_IOS)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_IOS
        PEACH_PLATFORM_APPLE
        PEACH_PLATFORM_MOBILE
        PEACH_ARCH_ARM64
    )

elseif(PEACH_TVOS)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_TVOS
        PEACH_PLATFORM_APPLE
        PEACH_ARCH_ARM64
    )

elseif(PEACH_LINUX)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_LINUX
        PEACH_PLATFORM_DESKTOP
        PEACH_ARCH_X64
    )

elseif(PEACH_BSD)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_BSD
        PEACH_PLATFORM_FREEBSD
        PEACH_PLATFORM_DESKTOP
        PEACH_ARCH_X64
    )

elseif(PEACH_HAIKU)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_HAIKU
        PEACH_PLATFORM_DESKTOP
        PEACH_ARCH_X64
    )

elseif(PEACH_ANDROID)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_ANDROID
        PEACH_PLATFORM_MOBILE
        PEACH_ARCH_ARM64
    )

elseif(PEACH_WASM)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_WASM
        PEACH_PLATFORM_WEB
        PEACH_ARCH_WASM32 #ik that wasm3 or w/e is 64 bit idfk owo uwu
    )

elseif(PEACH_VITA)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_VITA
        PEACH_PLATFORM_HANDHELD
        PEACH_ARCH_ARMV7
    )

elseif(PEACH_SWITCH)
    target_compile_definitions(starlight_physics PUBLIC
        PEACH_PLATFORM_SWITCH
        PEACH_PLATFORM_HANDHELD
        PEACH_ARCH_ARM64
    )

endif()

####################################### Compiler Options and Other Definitions

target_compile_definitions(starlight_physics PUBLIC 
    $<$<CONFIG:Debug>:STARLIGHT_DEBUG>
)

if(MSVC)

    target_compile_options(starlight_physics PRIVATE
        $<$<CONFIG:Debug>:/MTd>
        $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:/MT>
    ) 
    
    target_compile_options(starlight_physics PUBLIC 
        /utf-8
        /experimental:c11atomics
        /Zc:preprocessor
        /std:c17
    )

endif(MSVC)

if(BUILD_STARLIGHT_SHARED)
    target_compile_definitions(starlight_physics PRIVATE
        STARLIGHT_BUILD_DYNAMIC
    ) 
else()
    target_compile_definitions(starlight_physics PRIVATE
        STARLIGHT_API_STATIC #used for compiling peach_api as a static lib uwu
    ) 
endif(BUILD_STARLIGHT_SHARED)

####################################### Executable Targets

if(BUILD_STARLIGHT_TESTS)
    file(
        GLOB_RECURSE 
        STARLIGHT_PHYSICS_TESTS_SOURCES 
        CONFIGURE_DEPENDS 
        "tests/main.c" 
        "tests/Graphics.h"
    )

    ####################################### Create executable target

    add_executable(     
        starlight_tests
        ${STARLIGHT_PHYSICS_TESTS_SOURCES}
    )

    ####################################### helper macro for reducing repeated boiler plate for static linking uwu

    macro(STATIC_IMPORT TARGET RELEASE_LIB DEBUG_LIB INCLUDE_DIR)
        add_library(${TARGET} STATIC IMPORTED)
        set_target_properties(${TARGET} PROPERTIES
            IMPORTED_LOCATION_DEBUG           ${DEBUG_LIB}
            IMPORTED_LOCATION_RELEASE         ${RELEASE_LIB}
            IMPORTED_LOCATION_RELWITHDEBINFO  ${RELEASE_LIB}
            IMPORTED_LOCATION_MINSIZEREL      ${RELEASE_LIB}
            INTERFACE_INCLUDE_DIRECTORIES     ${INCLUDE_DIR}
        )
    endmacro()

    ####################################### Static Linkage

    if(PEACH_WINDOWS)
        # AND CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64"

        ############ GL extensions for legacy systems

        STATIC_IMPORT(glew 
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/win64/release/libglew32.lib"
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/win64/debug/libglew32d.lib"
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/include"
        )

        ############ Windowing and Input

        STATIC_IMPORT(SDL3 
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/win64/release/SDL3-static.lib"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/win64/debug/SDL3-static.lib"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/include"
        )

    elseif(PEACH_MACOS)

        ############ Windowing and Input

        STATIC_IMPORT(SDL3 
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/steve64/release/libSDL3.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/steve64/debug/libSDL3.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/include"
        )

    elseif(PEACH_LINUX) #XXX: doing these later when i compile on my linux machine, future me: WE DID BIIIITCH WE IN IT

        ############ GL extensions for legacy systems

        STATIC_IMPORT(glew 
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/tux64/libGLEW.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/tux64/libGLEWd.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/include"
        )

        ############ Windowing and Input

        STATIC_IMPORT(SDL3 
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/tux64/release/libSDL3.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/tux64/debug/libSDL3.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/include"
        )

    elseif(PEACH_BSD) #Xxx: doing this took decades off my life ;w;

        ############ GL extensions for legacy systems

        STATIC_IMPORT(glew 
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/freebsd64/libGLEW.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/freebsd64/libGLEWd.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/glew/include"
        )

        ############ Windowing and Input

        STATIC_IMPORT(SDL3 
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/freebsd64/release/libSDL3.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/freebsd64/debug/libSDL3.a"
            "${PROJECT_SOURCE_DIR}/tests/third_party/SDL3/include"
        )

    else()
        message(FATAL_ERROR "UNSUPPORTED TESTING PLATFORM STARLIGHT_TESTS IS ONLY SUPPORTED ON WINDOWS/LINUX/FREEBSD SORRY NOT SORRY")
    endif()

    ####################################### Dependency Linking

    if(PEACH_LINUX)

        find_package(OpenGL REQUIRED)

        target_link_libraries(starlight_tests PUBLIC
            #opengl
            GL
            glew    
            SDL3
            m #?????????????????????????????????????????????
        )

    elseif(PEACH_PLATFORM_IS_WINDOWS)

        find_package(OpenGL REQUIRED)

        target_link_libraries(starlight_tests PUBLIC
            #opengl
            opengl32
            glew    

            setupapi 
            version 
            imm32 
            winmm 
            kernel32

            SDL3
        )

    elseif(PEACH_MACOS) #TODO fix iokit and other frameworks that dont exist on ios ipados or tvos and watchos

        target_link_libraries(starlight_tests PUBLIC
            "-framework AVFoundation"
            "-framework AudioToolbox"
            "-framework CoreAudio"
            "-framework CoreHaptics"
            "-framework CoreVideo"
            "-framework Metal"
            "-framework MetalKit"
            "-framework QuartzCore"
            "-framework CoreGraphics"
            "-framework Foundation"
            "-framework IOKit"
            "-framework Cocoa"
            "-framework GameController"
            "-framework CoreMedia"
            "-framework ForceFeedback"
            "-framework Carbon" #wtf sdl3 u still use this garbo
            "-framework UniformTypeIdentifiers"
            "-lpthread"
        ) #needed for statically linking SDL3

    elseif(PEACH_BSD)

        # Gotta let the linker know where to find GL since bsd is a special little kitty >O<
        link_directories(/usr/local/lib)
        include_directories(/usr/local/include)

        find_package(OpenGL REQUIRED)

        target_link_libraries(starlight_tests PUBLIC
            GL
            glew
            usbhid # needed for SDL3
            pthread # FreeBSD uses clang by default, pthread is separate
            SDL3
        )

    elseif(PEACH_WASM)
        target_link_options(starlight_tests PUBLIC #wasm is weird mang, but I get it owo
            "-sMAX_WEBGL_VERSION=2" 
            "-sUSE_SDL=3" 
            SDL3
        )
    endif()

    ####################################### Compiler Options and Definitions

    if(MSVC)
        target_compile_options(starlight_tests PRIVATE
            $<$<CONFIG:Debug>:/MTd>
            $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:/MT>
        ) 
    endif(MSVC)

    ####################################### Link to Peach Core

    target_link_libraries(starlight_tests PRIVATE 
        starlight_physics
    )

    ####################################### Compiler Options and Other Definitions

    target_compile_definitions(starlight_tests PRIVATE 
        SDL_MAIN_HANDLED
    )

    if(PEACH_PLATFORM_IS_WINDOWS)

        target_compile_definitions(starlight_tests PRIVATE
            GLEW_STATIC #needed for statically linking glew
        )

        if(MSVC)
            target_compile_options(starlight_tests PRIVATE
                $<$<CONFIG:Debug>:/MTd>
                $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:/MT>
            ) 
            
            target_compile_options(starlight_tests PUBLIC 
                /utf-8
            )
        endif(MSVC)

    endif(PEACH_PLATFORM_IS_WINDOWS)

endif(BUILD_STARLIGHT_TESTS)

####################################### Set Startup Project (Visual Studio & Xcode)

# Set Peach_Editor as the default scheme in Xcode

if(CMAKE_GENERATOR MATCHES "Xcode" AND BUILD_STARLIGHT_TESTS)
    set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
    set(CMAKE_XCODE_SCHEME_EXECUTABLE starlight_tests)
    message(STATUS "Setting Peach_Editor as the default startup project for Xcode")
endif()

# Set Peach_Editor as the startup project for Visual Studio

if(CMAKE_GENERATOR MATCHES "Visual Studio" AND BUILD_STARLIGHT_TESTS)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT starlight_tests)
    message(STATUS "Setting starlight_tests as the default startup project for VS2022")
    set_property(TARGET starlight_tests PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "")
endif()
