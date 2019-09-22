message(STATUS "Aegisub Build: Collecting resources...")
file(GLOB_RECURSE AEGISUB_BITMAPS resources/*.png)
file(GLOB AEGISUB_CONFIGS resources/configs/*.json)
file(GLOB AEGISUB_CONFIGS_OSX resources/configs/osx/*.json)

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

string(CONFIGURE [[
#ifdef RESOURCE_H_
#define RESOURCE_H_

@AEGISUB_RES_WIN_ENTRIES@

#endif
]] AEGISUB_RES_WIN)
file(MAKE_DIRECTORY src/generated/windows)
file(MAKE_DIRECTORY src/generated/osx)
file(WRITE src/generated/windows/resource.h ${AEGISUB_RES_WIN})
file(WRITE src/generated/windows/resource.rc ${AEGISUB_RES_WIN_RC_ENTRIES})
file(WRITE src/generated/osx/manifest.respack ${AEGISUB_RES_RESPACK_ENTRIES})
file(WRITE src/generated/osx/manifest_osx.respack ${AEGISUB_RES_OSX_RESPACK_ENTRIES})
message(STATUS "Aegisub Build: Resources manifest generated")