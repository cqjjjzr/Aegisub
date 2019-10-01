message(STATUS "Aegisub Build: Collecting resources...")
file(GLOB_RECURSE AEGISUB_BITMAPS resources/*.png)
file(GLOB AEGISUB_CONFIGS resources/configs/*.json)
file(GLOB AEGISUB_CONFIGS_OSX resources/configs/osx/*.json)

if(WIN32)
    configure_file("${PROJECT_SOURCE_DIR}/resources/configs/default_config_win.json" "${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json" COPYONLY)
else()
    add_custom_command(
        OUTPUT ${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/resources/configs/default_config_win.json ${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json
    )
    configure_file("resources/configs/default_config_platform.json.in" "${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json" @ONLY)
endif()

set(AEGISUB_RES_WIN_ENTRIES "")
set(AEGISUB_RES_WIN_RC_ENTRIES "")
set(AEGISUB_RES_RESPACK_ENTRIES "")

foreach(BITMAP_FILENAME IN LISTS AEGISUB_BITMAPS)
  get_filename_component(BITMAP_NAME ${BITMAP_FILENAME} NAME_WE)
  string(APPEND AEGISUB_RES_WIN_ENTRIES "#define bitmap_${BITMAP_NAME}\n")
  string(APPEND AEGISUB_RES_WIN_RC_ENTRIES "bitmap_${BITMAP_NAME} RCDATA ${BITMAP_FILENAME}\n")
  string(APPEND AEGISUB_RES_RESPACK_ENTRIES "${BITMAP_FILENAME}\n")
endforeach()

foreach(CONFIG_FILENAME IN LISTS AEGISUB_CONFIGS)
  get_filename_component(CONFIG_NAME ${CONFIG_FILENAME} NAME_WE)
  string(APPEND AEGISUB_RES_WIN_ENTRIES "#define config_${CONFIG_NAME}\n")
  string(APPEND AEGISUB_RES_WIN_RC_ENTRIES "config_${CONFIG_NAME} RCDATA ${CONFIG_FILENAME}\n")
  string(APPEND AEGISUB_RES_RESPACK_ENTRIES "${CONFIG_FILENAME}\n")
endforeach()

set(AEGISUB_RES_OSX_RESPACK_ENTRIES ${AEGISUB_RES_RESPACK_ENTRIES})
foreach(CONFIG_FILENAME IN LISTS AEGISUB_CONFIGS_OSX)
  get_filename_component(CONFIG_NAME ${CONFIG_FILENAME} NAME_WE)
  string(APPEND AEGISUB_RES_OSX_RESPACK_ENTRIES "${CONFIG_FILENAME}\n")
endforeach()

file(MAKE_DIRECTORY src/generated/windows)
file(MAKE_DIRECTORY src/generated/osx)
configure_file(resources/resource.rc.in src/generated/windows/resource.rc)
configure_file(resources/manifest.respack.in src/generated/osx/manifest.respack)
configure_file(resources/manifest_osx.respack.in src/generated/osx/manifest_osx.respack)
message(STATUS "Aegisub Build: Resources manifest generated")