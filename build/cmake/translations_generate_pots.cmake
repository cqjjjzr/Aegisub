# This is a CMake script. Don't include this in CMakeLists.txt

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..) # build/cmake/
set(CMAKE_FIND_LIBRARY_SUFFIXES "") # shut up
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build/cmake/modules)

find_package(Gettext)
if(NOT GETTEXT_TOOLS_FOUND)
    message(FATAL_ERROR "Gettext tools not found!")
    message(FATAL_ERROR "For Windows users, you can download the Windows version of gettext: https://mlocati.github.io/articles/gettext-iconv-windows.html")
endif()

message(STATUS "Aegisub Build: Collecting source code to generate translate templates...")
file(GLOB_RECURSE TRANSLATION_SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)

list(JOIN TRANSLATION_SRCS "\n" TRANSLATION_SRCS_STR)
file(WRITE "translation_srcs.txt" ${TRANSLATION_SRCS_STR})

message(STATUS "Aegisub Build: Executing xgettext...")
execute_process(COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -f translation_srcs.txt -o aegisub.pot --c++ -k_ -kSTR_MENU -kSTR_DISP -kSTR_HELP -kfmt_tl -kfmt_plural:2,3 -kwxT -kwxPLURAL:1,2
                OUTPUT_VARIABLE rv)
message(${rv})

file(READ "aegisub.pot" GENERATED_POT)
string(REPLACE ${PROJECT_SOURCE_DIR} ".." GENERATED_POT ${GENERATED_POT})

macro(append_message msgfile msgline msgid)
    string(FIND "${GENERATED_POT}" "${msgid}" EXISTING_POS)
    if(EXISTING_POS EQUAL -1)
        string(APPEND GENERATED_POT "\n#: ${msgfile}:${msgline}\nmsgid \"${msgid}\"\nmsgstr \"\"\n")
    endif()
endmacro()

message(STATUS "Aegisub Build: Appending localizible strings from config files...")
macro(extract_json_text text msgfile expr) # CMake WCNM
    string(REPLACE "\\\"" "$QUOTE$" text "${text}")

    string(REGEX MATCHALL ${expr} TEXT_LINES "${text}")

    foreach(TEXT_LINE IN LISTS TEXT_LINES)
        #message(${TEXT_LINE})
        string(REGEX REPLACE ${expr} "\\1" TEXT_LINE "${TEXT_LINE}")
        string(REPLACE "$QUOTE$" "\\\"" TEXT_LINE "${TEXT_LINE}")
        append_message(${msgfile} 0 "${TEXT_LINE}")
    endforeach()
endmacro()

file(READ "${PROJECT_SOURCE_DIR}/resources/configs/default_menu.json" TOTAL_TEXT)
extract_json_text("${TOTAL_TEXT}" "default_menu.json" "\"text\"[ ]*:[ ]*\"([^\"]+)\"")
file(READ "${PROJECT_SOURCE_DIR}/resources/configs/osx/default_menu.json" TOTAL_TEXT)
extract_json_text("${TOTAL_TEXT}" "default_menu.json" "\"text\"[ ]*:[ ]*\"([^\"]+)\"")
file(READ "${PROJECT_SOURCE_DIR}/resources/configs/default_hotkey.json" TOTAL_TEXT)
extract_json_text("${TOTAL_TEXT}" "default_hotkey.json" "\"([^\"]+)\"[ \\n]*:[ \\n]{")
file(READ "${PROJECT_SOURCE_DIR}/resources/configs/default_hotkey.json" TOTAL_TEXT)
extract_json_text("${TOTAL_TEXT}" "default_hotkey.json" "\"([^\"]+)\"[ \\n]*:[ \\n]{")

message(STATUS "Aegisub Build: Appending localizible strings from automation Lua scripts...")
file(GLOB_RECURSE LUA_SCRIPTS ${PROJECT_SOURCE_DIR}/automation/*.lua)
foreach(LUA_SCRIPT_FILE IN LISTS LUA_SCRIPTS)
    # CMake has ruined my life!!!
    # CMake always messes with backslashes and newlines, especially when passing string arguments.
    # Code below is to fix "\n" and "\\<any_letter>" in the Lua script.
    # Be especially careful about the order of those commands.
    file(READ ${LUA_SCRIPT_FILE} TOTAL_TEXT)
    string(REPLACE "\\n" "$$$$NEWLINENEWLINENEWLINE$$$" TOTAL_TEXT ${TOTAL_TEXT})
    string(REPLACE "\\\\" "\\\\\\\\" TOTAL_TEXT "${TOTAL_TEXT}")
    string(REPLACE "$$$$NEWLINENEWLINENEWLINE$$$" "\\\\n" TOTAL_TEXT "${TOTAL_TEXT}")
    string(REPLACE "\\" "\\\\" TOTAL_TEXT "${TOTAL_TEXT}")

    string(REPLACE ${PROJECT_SOURCE_DIR} ".." LUA_SCRIPT_FILE ${LUA_SCRIPT_FILE})


    extract_json_text("${TOTAL_TEXT}" ${LUA_SCRIPT_FILE} "tr\"([^\"]+)\"")
endforeach()

message(STATUS "Aegisub Build: Appending localizible strings from installation scripts...")

file(READ "${PROJECT_SOURCE_DIR}/packages/win_installer/fragment_strings.iss.in" TOTAL_TEXT)
extract_json_text("${TOTAL_TEXT}" "packages/win_installer/fragment_strings.iss" "_[^=]+=([^\\n]+)")

file(WRITE "aegisub.pot" ${GENERATED_POT})
file(WRITE "aegisub2.pot" ${GENERATED_POT})
execute_process(COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} "${PROJECT_SOURCE_DIR}/packages/desktop/aegisub.desktop.template.in" -o aegisub.pot --language=Desktop --join-existing
                OUTPUT_VARIABLE rv)
message(${rv})

file(READ "aegisub.pot" GENERATED_POT)
string(REPLACE "${PROJECT_SOURCE_DIR}/packages/desktop/aegisub.desktop.template.in" "aegisub.desktop" GENERATED_POT ${GENERATED_POT})

# TODO Process aegisub.appdata.xml.template.in
# This has not been yet implemented since most Aegisub releases don't contain it.
# And also the CMake's regex supporting sucks.

#file(READ "${PROJECT_SOURCE_DIR}/packages/win_installer/aegisub.appdata.xml.template.in" TOTAL_TEXT)
#string(REPLACE "\n" "" TOTAL_TEXT ${TOTAL_TEXT})
#extract_json_text("${TOTAL_TEXT}" "packages/win_installer/fragment_strings.iss" "<_[]>")

set(NO_GENERATE_GIT_VERSION_H YES)
message(STATUS "Aegisub Build: Using Git to determine project version...")
include(${CMAKE_CURRENT_LIST_DIR}/git_version.cmake)
message(STATUS "Aegisub Build: Project version: ${INSTALLER_VERSION_STRING}")

string(REPLACE "SOME DESCRIPTIVE TITLE." "Aegisub ${INSTALLER_VERSION_STRING}" GENERATED_POT ${GENERATED_POT})
string(TIMESTAMP CURR_YEAR "%Y" UTC)
string(REPLACE "YEAR" "2005-${CURR_YEAR}" GENERATED_POT ${GENERATED_POT})
string(REPLACE "THE PACKAGE'S COPYRIGHT HOLDER" "Rodrigo Braz Monteiro, Niels Martin Hansen, Thomas Goyne et. al." GENERATED_POT ${GENERATED_POT})
string(REPLACE "PACKAGE" "Aegisub" GENERATED_POT ${GENERATED_POT})

string(REPLACE "VERSION" ${INSTALLER_VERSION_STRING} GENERATED_POT ${GENERATED_POT})
string(REPLACE "FIRST AUTHOR <EMAIL@ADDRESS>" "Niels Martin Hansen <nielsm@aegisub.org>" GENERATED_POT ${GENERATED_POT})
string(REPLACE "CHARSET" "UTF-8" GENERATED_POT ${GENERATED_POT})

file(WRITE "${PROJECT_SOURCE_DIR}/po/aegisub.pot" ${GENERATED_POT})

message(STATUS "Aegisub Build: Successfully generated translate templates.")
