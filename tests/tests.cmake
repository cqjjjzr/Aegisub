if(UNIX)
    add_executable(aegisub-lua EXCLUDE_FROM_ALL
        automation/tests/aegisub.cpp
    )
    target_include_directories(aegisub-lua PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_directories(aegisub-lua PRIVATE ${Boost_LIBRARY_DIRS})
    target_link_libraries(aegisub-lua libaegisub luabins luajit ${Boost_LIBRARIES} ${ICU_LIBRARIES})
    add_custom_target(test-automation
        COMMAND sh -c "$(luarocks path); ${PROJECT_BINARY_DIR}/aegisub-lua tests/busted.lua -p moon tests/modules"
        VERBATIM
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/automation"
    )
    add_dependencies(test-automation aegisub-lua)
else()
    add_custom_target(test-automation)
endif()

message(STATUS "Aegisub Build: Configuring tests...")
include(GoogleTest)
enable_testing()

add_executable(test-aegisub EXCLUDE_FROM_ALL
    tests/tests/access.cpp
    tests/tests/audio.cpp
    tests/tests/cajun.cpp
    tests/tests/calltip_provider.cpp
    tests/tests/character_count.cpp
    tests/tests/color.cpp
    tests/tests/dialogue_lexer.cpp
    tests/tests/format.cpp
    tests/tests/fs.cpp
    tests/tests/hotkey.cpp
    tests/tests/iconv.cpp
    tests/tests/ifind.cpp
    tests/tests/karaoke_matcher.cpp
    tests/tests/keyframe.cpp
    tests/tests/line_iterator.cpp
    tests/tests/line_wrap.cpp
    tests/tests/mru.cpp
    tests/tests/option.cpp
    tests/tests/path.cpp
    tests/tests/signals.cpp
    tests/tests/split.cpp
    tests/tests/syntax_highlight.cpp
    tests/tests/thesaurus.cpp
    tests/tests/time.cpp
    tests/tests/type_name.cpp
    tests/tests/util.cpp
    tests/tests/uuencode.cpp
    tests/tests/vfr.cpp
    tests/tests/word_split.cpp
    tests/support/main.cpp
    tests/support/util.cpp
)

target_compile_definitions(test-aegisub PRIVATE CMAKE_BUILD)
target_include_directories(test-aegisub PRIVATE tests/support)
target_link_libraries(test-aegisub PRIVATE libaegisub)
include(${DEPENDENCIES_CMAKE_FILE_TEST}) # Default: build/cmake/dependencies_tests.cmake

if(MSVC)
    target_compile_options(test-aegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/tests/support/tests_pre.h")
else()
    target_compile_options(test-aegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/tests/support/tests_pre.h")
endif()

message(STATUS "Aegisub Build: Bootstrapping tests, preparing datas...")
add_custom_target(test-setup)
if(WIN32)
    add_custom_command(
        TARGET test-setup
        COMMAND "${PROJECT_SOURCE_DIR}/tests/setup.bat"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
    )
else()
    add_custom_command(
        TARGET test-setup
        COMMAND "${PROJECT_SOURCE_DIR}/tests/setup.sh"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
    )
endif()

gtest_add_tests(TARGET test-aegisub
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests")

message(STATUS "Aegisub Build: Test prepared.")

#add_custom_target(test DEPENDS test-automation =)
