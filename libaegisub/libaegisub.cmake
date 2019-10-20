add_library(libaegisub STATIC
    libaegisub/common/parser.cpp
    libaegisub/ass/dialogue_parser.cpp
    libaegisub/ass/time.cpp
    libaegisub/ass/uuencode.cpp
    libaegisub/audio/provider.cpp
    libaegisub/audio/provider_convert.cpp
    libaegisub/audio/provider_dummy.cpp
    libaegisub/audio/provider_hd.cpp
    libaegisub/audio/provider_lock.cpp
    libaegisub/audio/provider_pcm.cpp
    libaegisub/audio/provider_ram.cpp
    libaegisub/common/cajun/elements.cpp
    libaegisub/common/cajun/reader.cpp
    libaegisub/common/cajun/writer.cpp
    libaegisub/lua/modules/lfs.cpp
    libaegisub/lua/modules/re.cpp
    libaegisub/lua/modules/unicode.cpp
    libaegisub/lua/modules/lpeg.c
    libaegisub/lua/modules.cpp
    libaegisub/lua/script_reader.cpp
    libaegisub/lua/utils.cpp
    libaegisub/common/calltip_provider.cpp
    libaegisub/common/character_count.cpp
    libaegisub/common/charset.cpp
    libaegisub/common/charset_6937.cpp
    libaegisub/common/charset_conv.cpp
    libaegisub/common/color.cpp
    libaegisub/common/file_mapping.cpp
    libaegisub/common/format.cpp
    libaegisub/common/fs.cpp
    libaegisub/common/hotkey.cpp
    libaegisub/common/io.cpp
    libaegisub/common/json.cpp
    libaegisub/common/kana_table.cpp
    libaegisub/common/karaoke_matcher.cpp
    libaegisub/common/keyframe.cpp
    libaegisub/common/line_iterator.cpp
    libaegisub/common/log.cpp
    libaegisub/common/mru.cpp
    libaegisub/common/option.cpp
    libaegisub/common/option_value.cpp
    libaegisub/common/path.cpp
    libaegisub/common/thesaurus.cpp
    libaegisub/common/util.cpp
    libaegisub/common/vfr.cpp
    libaegisub/common/ycbcr_conv.cpp
    libaegisub/common/dispatch.cpp
    libaegisub/common/resources_fs.cpp
)

if(UNIX)
    target_sources(libaegisub PRIVATE
        libaegisub/unix/access.cpp
        libaegisub/unix/fs.cpp
        libaegisub/unix/log.cpp
        libaegisub/unix/path.cpp
        libaegisub/unix/util.cpp
        libaegisub/unix/resources.cpp
    )
    set_property(
        SOURCE libaegisub/unix/path.cpp
        PROPERTY COMPILE_DEFINITIONS
            P_DATA="${CMAKE_INSTALL_PREFIX}/share/aegisub/"
    )
elseif(WIN32)
    target_sources(libaegisub PRIVATE
        libaegisub/windows/access.cpp
        libaegisub/windows/charset_conv_win.cpp
        libaegisub/windows/fs.cpp
        libaegisub/windows/lagi_pre.cpp
        libaegisub/windows/log_win.cpp
        libaegisub/windows/path_win.cpp
        libaegisub/windows/util_win.cpp
        libaegisub/windows/resources_win.cpp
    )
endif()
if(APPLE)
    target_sources(libaegisub PRIVATE
        libaegisub/osx/dispatch.mm
        libaegisub/osx/spellchecker.mm
        libaegisub/osx/util.mm
    )
endif()
set_target_properties(libaegisub PROPERTIES PREFIX "")
target_compile_definitions(libaegisub PRIVATE CMAKE_BUILD)

if (MSVC)
    target_compile_options(libaegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/libaegisub/lagi_pre.h")
else(MSVC)
    target_compile_options(libaegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/libaegisub/lagi_pre.h")
endif()
