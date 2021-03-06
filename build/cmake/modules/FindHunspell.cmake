# - Try to find Hunspell
# Once done this will define
#
#  HUNSPELL_FOUND - system has Hunspell
#  HUNSPELL_INCLUDE_DIR - the Hunspell include directory
#  HUNSPELL_LIBRARIES - Link these to use Hunspell
#  HUNSPELL_HAS_STRING_API - Hunspell has vector<string> api (>=1.5.1)
#
# Redistribution and use of this file is allowed according to the terms of the
# MIT license. For details see the file COPYING-CMAKE-MODULES.


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls

find_package(PkgConfig QUIET)

pkg_check_modules(HUNSPELL_PKG QUIET hunspell)

FIND_PATH(HUNSPELL_INCLUDE_DIR NAMES hunspell.h
  PATHS
    /usr/local/include
    /usr/include
  HINTS
    ${HUNSPELL_PKG_INCLUDE_DIRS} # Generated by pkg-config
  PATH_SUFFIXES hunspell
)

FIND_LIBRARY(HUNSPELL_LIBRARIES NAMES hunspell-1.7 hunspell-1.6 hunspell-1.5 hunspell-1.4 hunspell-1.3 hunspell-1.2 hunspell ${HUNSPELL_PKG_LIBRARIES}
  PATHS
    /usr/local
    /usr
  HINTS
    ${HUNSPELL_PKG_LIBRARY_DIRS} # Generated by pkg-config
  PATH_SUFFIXES
    lib64
    lib
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Hunspell DEFAULT_MSG HUNSPELL_LIBRARIES HUNSPELL_INCLUDE_DIR )

add_library(hunspell UNKNOWN IMPORTED)
set_target_properties(hunspell PROPERTIES IMPORTED_LOCATION ${HUNSPELL_LIBRARIES} INTERFACE_INCLUDE_DIRECTORIES ${HUNSPELL_INCLUDE_DIR})
if (NOT BUILD_SHARED_LIBS)
  # At least statically compiled hunspell 1.7.0 requires HUNSPELL_STATIC
  # For other versions, it should not hurt
  set_target_properties(hunspell PROPERTIES INTERFACE_COMPILE_DEFINITIONS HUNSPELL_STATIC)
endif ()

if (HUNSPELL_FOUND)
  try_compile(HUNSPELL_HAS_STRING_API "${CMAKE_BINARY_DIR}/hunspell_string_api"
              "${CMAKE_CURRENT_LIST_DIR}/hunspell_string_api.cpp"
              LINK_LIBRARIES ${HUNSPELL_LIBRARIES}
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${HUNSPELL_INCLUDE_DIR}")
  if (HUNSPELL_HAS_STRING_API)
    message(STATUS "Hunspell has string API")
  else(HUNSPELL_HAS_STRING_API)
    message(STATUS "Hunspell does not have string API")
  endif(HUNSPELL_HAS_STRING_API)
endif(HUNSPELL_FOUND)

# show the HUNSPELL_INCLUDE_DIR and HUNSPELL_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED(HUNSPELL_INCLUDE_DIR HUNSPELL_LIBRARIES HUNSPELL_HAS_STRING_API)
