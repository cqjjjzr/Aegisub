# This is to be included in CMakeLists.txt

find_package(Gettext)
if(NOT GETTEXT_TOOLS_FOUND)
    message(FATAL_ERROR "Gettext tools not found!")
endif()

message(STATUS "Aegisub Build: Collecting po file to compile translations...")
file(GLOB TRANSLATION_PO_FILES ${PROJECT_SOURCE_DIR}/po/*.po)

set(AEGISUB_CATALOG "aegisub" CACHE STRING "Name of the Aegisub gettext catalog.")

set(MO_FILES "")
foreach(PO_FILE IN LISTS TRANSLATION_PO_FILES)
    get_filename_component(LANG ${PO_FILE} NAME_WE)
    set(MO_FILE "${CMAKE_CURRENT_BINARY_DIR}/${LANG}.mo")

    list(APPEND MO_FILES "${MO_FILE}")
    add_custom_command(
        OUTPUT "${LANG}.mo"
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE}
            -o "${MO_FILE}"
            "${PO_FILE}"
        DEPENDS "${PO_FILE}")
    if(APPLE)
        install(FILES "${MO_FILE}"
            RENAME "${AEGISUB_CATALOG}.mo"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/Resources/${LANG}.proj/")
    else()
        install(FILES "${MO_FILE}"
            RENAME "${AEGISUB_CATALOG}.mo"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/locale/${LANG}/LC_MESSAGES/")
    endif()
endforeach()

add_custom_target(translations ALL DEPENDS ${MO_FILES})
