# This is a CMake script. Don't include this in CMakeLists.txt

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..) # build/cmake/
set(CMAKE_FIND_LIBRARY_SUFFIXES "") # shut up
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build/cmake/modules)

find_package(Gettext)
if(NOT GETTEXT_TOOLS_FOUND)
    message(FATAL_ERROR "Gettext tools not found!")
endif()

file(STRINGS "${PROJECT_SOURCE_DIR}/po/LINGUAS" LINGUAS
      REGEX "^[^#].*")
string(REGEX MATCHALL "[^ ]+" languages ${LINGUAS})

foreach(LANGUAGE IN LISTS languages)
    execute_process(COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --verbose ${LANGUAGE}
                OUTPUT_VARIABLE rv
                ERROR_VARIABLE err)
    if("${err}" MATCHES "([0-9]+)[^0-9]*([0-9]+)?[^0-9]*([0-9]+)?")
        set(NUM_TRANSLATED_STRINGS ${CMAKE_MATCH_1})
        set(NUM_STRINGS ${NUM_TRANSLATED_STRINGS})
        if (NOT ${CMAKE_MATCH_2} STREQUAL "")
            math(EXPR NUM_STRINGS "${NUM_STRINGS} + ${CMAKE_MATCH_2}")
        endif()
        if (NOT ${CMAKE_MATCH_3} STREQUAL "")
            math(EXPR NUM_STRINGS "${NUM_STRINGS} + ${CMAKE_MATCH_3}")
        endif()

        math(EXPR PERCENT "${NUM_TRANSLATED_STRINGS}*100/${NUM_STRINGS}")

        # pad string.
        string(LENGTH ${LANGUAGE} LEN)
        math(EXPR PAD "10-${LEN}")
        if (PAD GREATER 0)
            set(PADSTR "")
            foreach(FOO RANGE ${PAD})
                string(APPEND PADSTR " ")
            endforeach()
            string(APPEND LANGUAGE ${PADSTR})
        endif()
        message("${LANGUAGE}Total Strings:${NUM_STRINGS}, ${PERCENT}% translated.")
    endif()
endforeach()
