# Dependencies configurator
set(DEPENDENCIES_CMAKE_FILE "build/cmake/dependencies.cmake" CACHE STRING "CMake file that configures dependencies. The default one uses find_package.")
set(DEPENDENCIES_CMAKE_FILE_TEST "build/cmake/dependencies_tests.cmake" CACHE STRING "CMake file that configures dependencies in tests. The default one uses find_package. NOTICE: This should be set in DEPENDENCIES_CMAKE_FILE.")

# Subtitle dependencies
set(WITH_CSRI ON CACHE BOOL "Enable CSRI support")

# Video dependencies
set(WITH_FFMS2 ON CACHE BOOL "Enable FFMS2 support")
set(WITH_AVISYNTH ON CACHE BOOL "Enable AviSynth support")

# Audio dependencies
set(WITH_LIBPULSE ON CACHE BOOL "Enable PulseAudio support")
set(WITH_OPENAL ON CACHE BOOL "Enable OpenAL support")
set(WITH_OSS OFF CACHE BOOL "Enable OSS support")
set(WITH_PORTAUDIO ON CACHE BOOL "Enable PortAudio support")
set(WITH_ALSA ON CACHE BOOL "Enable ALSA support")
if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(WITH_ALSA OFF)
endif()
if(WIN32)
    set(WITH_DIRECTSOUND ON)

    set(WITH_OPENAL OFF)
    set(WITH_PORTAUDIO OFF)
    set(WITH_OSS OFF)
else()
    set(WITH_DIRECTSOUND OFF)
endif()

# Misc dependencies
set(WITH_HUNSPELL ON CACHE BOOL "Enable Hunspell support")
set(WITH_UCHARDET ON CACHE BOOL "Enable uchardet support")
set(WITH_FFTW3 ON CACHE BOOL "Enable fftw support")
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(WITH_FONTCONFIG ON)
else()
    set(WITH_FONTCONFIG OFF)
endif()

# Misc options

set(WITH_STARTUPLOG OFF CACHE BOOL "Enable startup log")
if(WITH_STARTUPLOG)
    target_compile_definitions(Aegisub PRIVATE "WITH_STARTUPLOG")
endif()

set(WITH_UPDATE_CHECKER OFF)
if(WITH_UPDATE_CHECKER)
    set(UPDATE_CHECKER_SERVER "\"updates.aegisub.org\"" CACHE STRING "Server for the update checker")
    set(UPDATE_CHECKER_BASE_URL "\"/trunk\"" CACHE STRING "Base path for the update checker")
    target_compile_definitions(Aegisub PRIVATE "WITH_UPDATE_CHECKER" "UPDATE_CHECKER_SERVER=${UPDATE_CHECKER_SERVER}" "UPDATE_CHECKER_BASE_URL=${UPDATE_CHECKER_BASE_URL}")
    target_link_libraries(Aegisub ${Boost_asio_LIBRARY})
    target_sources(Aegisub PRIVATE src/dialog_version_check.cpp)
endif()

set(WITH_BUILD_CREDIT OFF CACHE BOOL "Whether show build credit in about dialog")
if(WITH_BUILD_CREDIT)
    set(BUILD_CREDIT "" CACHE STRING "Build credit shown in about dialog")
    target_compile_definitions(Aegisub PRIVATE "BUILD_CREDIT=${BUILD_CREDIT}")
else()
    unset(BUILD_CREDIT CACHE)
endif()

set(WITH_TEST OFF CACHE BOOL "Enable unit tests")
# We couldn't include tests.cmake here.

set(WITH_TRANSLATION OFF CACHE BOOL "Build gettext translations. Require gettext tools.")
if(WITH_TRANSLATION)
    include(build/cmake/translations_build.cmake)
endif()