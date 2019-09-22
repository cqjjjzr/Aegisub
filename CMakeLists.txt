cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 17)

project(Aegisub)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/dependencies.cmake)
include(cmake/resources.cmake)

include_directories("libaegisub/include")

add_subdirectory(dependencies/csri)
include_directories(dependencies/csri/include)

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
)
if(UNIX)
    target_sources(libaegisub PRIVATE
        libaegisub/unix/access.cpp
        libaegisub/unix/fs.cpp
        libaegisub/unix/log.cpp
        libaegisub/unix/path.cpp
        libaegisub/unix/util.cpp
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
set_target_properties(libaegisub PROPERTIES PREFIX "")
target_compile_definitions(libaegisub PRIVATE CMAKE_BUILD)

add_executable(Aegisub WIN32
    src/command/app.cpp
    src/command/audio.cpp
    src/command/automation.cpp
    src/command/command.cpp
    src/command/edit.cpp
    src/command/grid.cpp
    src/command/help.cpp
    src/command/keyframe.cpp
    src/command/recent.cpp
    src/command/subtitle.cpp
    src/command/time.cpp
    src/command/timecode.cpp
    src/command/tool.cpp
    src/command/video.cpp
    src/command/vis_tool.cpp
    src/dialog_about.cpp
    src/dialog_align.cpp
    src/dialog_attachments.cpp
    src/dialog_automation.cpp
    src/dialog_autosave.cpp
    src/dialog_colorpicker.cpp
    src/dialog_detached_video.cpp
    src/dialog_dummy_video.cpp
    src/dialog_export.cpp
    src/dialog_export_ebu3264.cpp
    src/dialog_fonts_collector.cpp
    src/dialog_jumpto.cpp
    src/dialog_kara_timing_copy.cpp
    src/dialog_log.cpp
    src/dialog_paste_over.cpp
    src/dialog_progress.cpp
    src/dialog_properties.cpp
    src/dialog_resample.cpp
    src/dialog_search_replace.cpp
    src/dialog_selected_choices.cpp
    src/dialog_selection.cpp
    src/dialog_shift_times.cpp
    src/dialog_spellchecker.cpp
    src/dialog_style_editor.cpp
    src/dialog_style_manager.cpp
    src/dialog_styling_assistant.cpp
    src/dialog_text_import.cpp
    src/dialog_timing_processor.cpp
    src/dialog_translation.cpp
    src/dialog_video_details.cpp
    src/dialog_video_properties.cpp
    src/subtitle_format.cpp
    src/subtitle_format_ass.cpp
    src/subtitle_format_ebu3264.cpp
    src/subtitle_format_encore.cpp
    src/subtitle_format_microdvd.cpp
    src/subtitle_format_mkv.cpp
    src/subtitle_format_srt.cpp
    src/subtitle_format_ssa.cpp
    src/subtitle_format_transtation.cpp
    src/subtitle_format_ttxt.cpp
    src/subtitle_format_txt.cpp
    src/visual_tool.cpp
    src/visual_tool_clip.cpp
    src/visual_tool_cross.cpp
    src/visual_tool_drag.cpp
    src/visual_tool_rotatexy.cpp
    src/visual_tool_rotatez.cpp
    src/visual_tool_scale.cpp
    src/visual_tool_vector_clip.cpp
    src/MatroskaParser.c
    src/aegisublocale.cpp
    src/ass_attachment.cpp
    src/ass_dialogue.cpp
    src/ass_entry.cpp
    src/ass_export_filter.cpp
    src/ass_exporter.cpp
    src/ass_file.cpp
    src/ass_karaoke.cpp
    src/ass_override.cpp
    src/ass_parser.cpp
    src/ass_style.cpp
    src/ass_style_storage.cpp
    src/async_video_provider.cpp
    src/audio_box.cpp
    src/audio_colorscheme.cpp
    src/audio_controller.cpp
    src/audio_display.cpp
    src/audio_karaoke.cpp
    src/audio_marker.cpp
    src/audio_player.cpp
    src/audio_provider_factory.cpp
    src/audio_renderer.cpp
    src/audio_renderer_spectrum.cpp
    src/audio_renderer_waveform.cpp
    src/audio_timing_dialogue.cpp
    src/audio_timing_karaoke.cpp
    src/auto4_base.cpp
    src/auto4_lua.cpp
    src/auto4_lua_assfile.cpp
    src/auto4_lua_dialog.cpp
    src/auto4_lua_progresssink.cpp
    src/base_grid.cpp
    src/charset_detect.cpp
    src/colorspace.cpp
    src/colour_button.cpp
    src/compat.cpp
    src/context.cpp
    src/export_fixstyle.cpp
    src/export_framerate.cpp
    src/fft.cpp
    src/font_file_lister.cpp
    src/frame_main.cpp
    src/gl_text.cpp
    src/gl_wrap.cpp
    src/grid_column.cpp
    src/help_button.cpp
    src/hotkey.cpp
    src/hotkey_data_view_model.cpp
    src/image_position_picker.cpp
    src/initial_line_state.cpp
    src/main.cpp
    src/menu.cpp
    src/mkv_wrap.cpp
    src/pen.cpp
    src/persist_location.cpp
    src/preferences.cpp
    src/preferences_base.cpp
    src/project.cpp
    src/resolution_resampler.cpp
    src/search_replace_engine.cpp
    src/selection_controller.cpp
    src/spellchecker.cpp
    src/spline.cpp
    src/spline_curve.cpp
    src/string_codec.cpp
    src/subs_controller.cpp
    src/subs_edit_box.cpp
    src/subs_edit_ctrl.cpp
    src/subs_preview.cpp
    src/subtitles_provider.cpp
    src/subtitles_provider_libass.cpp
    src/text_file_reader.cpp
    src/text_file_writer.cpp
    src/text_selection_controller.cpp
    src/thesaurus.cpp
    src/timeedit_ctrl.cpp
    src/toggle_bitmap.cpp
    src/toolbar.cpp
    src/tooltip_manager.cpp
    src/utils.cpp
    src/validators.cpp
    src/vector2d.cpp
    src/version.cpp
    src/video_box.cpp
    src/video_controller.cpp
    src/video_display.cpp
    src/video_frame.cpp
    src/video_out_gl.cpp
    src/video_provider_cache.cpp
    src/video_provider_dummy.cpp
    src/video_provider_manager.cpp
    src/video_provider_yuv4mpeg.cpp
    src/video_slider.cpp
    src/visual_feature.cpp
)
target_link_libraries(Aegisub ${CMAKE_DL_LIBS} libaegisub csri)
conan_target_link_libraries(Aegisub)
target_compile_definitions(Aegisub PRIVATE CMAKE_BUILD)

set(WITH_BUILD_CREDIT OFF CACHE BOOL "Whether show build credit in about dialog")
if(WITH_BUILD_CREDIT)
    set(BUILD_CREDIT "" CACHE STRING "Build credit shown in about dialog")
    target_compile_definitions(Aegisub PRIVATE "BUILD_CREDIT=${BUILD_CREDIT}")
else()
    unset(BUILD_CREDIT CACHE)
endif()

if (MSVC)
    target_compile_options(libaegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/libaegisub/lagi_pre.h")
else(MSVC)
    target_compile_options(libaegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/libaegisub/lagi_pre.h")
endif()

if (MSVC)
    add_definitions("-DNOMINMAX -DINITGUID")
    target_compile_options(Aegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/src/agi_pre.h" "-MP")
    target_link_libraries (Aegisub Usp10)
    #target_sources(Aegisub PRIVATE src/res/res.rc src/res/strings.rc src/crash_writer_minidump.cpp)
    target_sources(Aegisub PRIVATE src/res/res.rc src/res/strings.rc src/generated/windows/resource.rc src/crash_writer.cpp src/dpi_aware.manifest)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Aegisub)
else()
    target_sources(Aegisub PRIVATE src/crash_writer.cpp)
    target_compile_options(Aegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/src/agi_pre.h")
endif()

if(WIN32)
    target_sources(Aegisub PRIVATE src/font_file_lister_gdi.cpp)
else (WIN32)
    target_sources(Aegisub PRIVATE src/font_file_lister_fontconfig.cpp)
endif()

find_package(OpenGL REQUIRED)
target_include_directories(Aegisub PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Aegisub ${OPENGL_LIBRARIES})

add_definitions("-DWITH_HUNSPELL")
target_sources(Aegisub PRIVATE src/spellchecker_hunspell.cpp)

add_definitions("-DHAVE_ICONV")

set(WITH_ALSA ON CACHE BOOL "Enable ALSA support")
if(WITH_ALSA)
    find_package(ALSA)
    if(NOT ALSA_FOUND)
        set(WITH_ALSA OFF CACHE BOOL "Enable ALSA support" FORCE)
    endif()
endif()
if(WITH_ALSA)
    target_compile_definitions(Aegisub PRIVATE "WITH_ALSA")
    target_include_directories(Aegisub PRIVATE ${ALSA_INCLUDE_DIRS})
    target_link_libraries(Aegisub ${ALSA_LIBRARIES})
    target_sources(Aegisub PRIVATE src/audio_player_alsa.cpp)
endif()

set(WITH_AVISYNTH ON CACHE BOOL "Enable AviSynth support")
if(WITH_AVISYNTH)
    find_package(AviSynth)
    if(NOT AviSynth_FOUND)
        set(WITH_AVISYNTH OFF CACHE BOOL "Enable AviSynth support" FORCE)
    endif()
endif()
if(WITH_AVISYNTH)
    target_compile_definitions(Aegisub PRIVATE "WITH_AVISYNTH" "AVS_LINKAGE_DLLIMPORT")
    target_include_directories(Aegisub PRIVATE ${AviSynth_INCLUDE_DIRS})
    target_link_libraries(Aegisub Vfw32 ${AviSynth_LIBRARIES})
    target_sources(Aegisub PRIVATE src/audio_provider_avs.cpp src/avisynth_wrap.cpp src/video_provider_avs.cpp)
endif()

target_compile_definitions(Aegisub PRIVATE "WITH_CSRI")
target_sources(Aegisub PRIVATE src/subtitles_provider_csri.cpp)
set_property(SOURCE src/subtitles_provider_csri.cpp PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/vendor/csri/include")

if(WIN32)
    target_compile_definitions(Aegisub PRIVATE "WITH_DIRECTSOUND")
    target_link_libraries(Aegisub dsound)
    target_sources(Aegisub PRIVATE src/audio_player_dsound.cpp src/audio_player_dsound2.cpp)
    set(WITH_DIRECTSOUND ON)
else()
    set(WITH_DIRECTSOUND OFF)
endif()

add_definitions("-DWITH_FFMS2")
target_sources(Aegisub PRIVATE src/audio_provider_ffmpegsource.cpp src/ffmpegsource_common.cpp src/video_provider_ffmpegsource.cpp)
add_definitions("-DWITH_FFTW3")

set(WITH_LIBPULSE ON CACHE BOOL "Enable PulseAudio support")
if(WITH_LIBPULSE)
    find_package(PulseAudio)
    if(NOT PULSEAUDIO_FOUND)
        set(WITH_LIBPULSE OFF CACHE BOOL "Enable PulseAudio support" FORCE)
    endif()
endif()
if(WITH_LIBPULSE)
    target_compile_definitions(Aegisub PRIVATE "WITH_LIBPULSE")
    target_include_directories(Aegisub PRIVATE ${PULSEAUDIO_INCLUDE_DIR})
    target_link_libraries(Aegisub ${PULSEAUDIO_LIBRARY})
    target_sources(Aegisub PRIVATE src/audio_player_pulse.cpp)
endif()

set(WITH_OPENAL ON CACHE BOOL "Enable OpenAL support")
if(WITH_OPENAL)
    find_package(OpenAL)
    if(NOT OPENAL_FOUND)
        set(WITH_OPENAL OFF CACHE BOOL "Enable OpenAL support" FORCE)
    endif()
endif()
if(WITH_OPENAL)
    target_compile_definitions(Aegisub PRIVATE "WITH_OPENAL")
    target_include_directories(Aegisub PRIVATE ${OPENAL_INCLUDE_DIR})
    target_link_libraries(Aegisub ${OPENAL_LIBRARY})
    target_sources(Aegisub PRIVATE src/audio_player_openal.cpp)
endif()

set(WITH_OSS OFF CACHE BOOL "Enable OSS support")
if(WITH_OSS)
    find_package(OSS)
    if(NOT OSS_FOUND)
        set(WITH_OSS OFF CACHE BOOL "Enable OSS support" FORCE)
    endif()
endif()
if(WITH_OSS)
    target_compile_definitions(Aegisub PRIVATE "WITH_OSS")
    target_include_directories(Aegisub PRIVATE ${OSS_INCLUDE_DIRS})
    target_sources(Aegisub PRIVATE src/audio_player_oss.cpp)
endif()

set(WITH_PORTAUDIO ON CACHE BOOL "Enable PortAudio support")
if(WITH_PORTAUDIO)
    find_package(PortAudio)
    if(NOT PortAudio_FOUND)
        set(WITH_PORTAUDIO OFF CACHE BOOL "Enable PortAudio support" FORCE)
    endif()
endif()
if(WITH_PORTAUDIO)
    target_compile_definitions(Aegisub PRIVATE "WITH_PORTAUDIO")
    target_include_directories(Aegisub PRIVATE ${PortAudio_INCLUDE_DIRS})
    target_link_libraries(Aegisub ${PortAudio_LIBRARIES})
    target_sources(Aegisub PRIVATE src/audio_player_portaudio.cpp)
endif()

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
add_definitions("-DWITH_UCHARDET")

if(WIN32)
    set(DEFAULT_PLAYER_AUDIO DirectSound)
    configure_file("${PROJECT_SOURCE_DIR}/resources/configs/default_config_win.json" "${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json" COPYONLY)
else()
    if(WITH_LIBPULSE)
        set(DEFAULT_PLAYER_AUDIO "PulseAudio" CACHE STRING "Default audio player")
    elseif(WITH_ALSA)
        set(DEFAULT_PLAYER_AUDIO "ALSA" CACHE STRING "Default audio player")
    elseif(WITH_OPENAL)
        set(DEFAULT_PLAYER_AUDIO "OpenAL" CACHE STRING "Default audio player")
    elseif(WITH_PORTAUDIO)
        set(DEFAULT_PLAYER_AUDIO "PortAudio" CACHE STRING "Default audio player")
    elseif(WITH_OSS)
        set(DEFAULT_PLAYER_AUDIO "OSS" CACHE STRING "Default audio player")
    else()
        message(SEND_ERROR
            "No supported audio player interface was enabled.\n"
            "If you want audio support in Aegisub you need to install one of these libraries:\n"
            " - PulseAudio\n"
            "   * http://pulseaudio.org/\n"
            " - ALSA (Linux only)\n"
            "   * http://www.alsa-project.org/\n"
            " - PortAudio (version 19 only)\n"
            "   * http://www.portaudio.com/\n"
            "\n"
        )
        set(DEFAULT_PLAYER_AUDIO "NONE" CACHE STRING "Default audio player")
    endif()
    add_custom_command(
        OUTPUT ${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/resources/configs/default_config_win.json ${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json
    )
    configure_file("resources/configs/default_config_platform.json.in" "${PROJECT_SOURCE_DIR}/resources/configs/default_config_platform.json" @ONLY)
endif()

if(NOT WIN32)
    set(AEGISUB_COMMAND "aegisub" CACHE STRING "The executable name of Aegisub")
    set_target_properties(Aegisub PROPERTIES OUTPUT_NAME "${AEGISUB_COMMAND}")
    configure_file("packages/desktop/aegisub.desktop.template.in" "aegisub.desktop" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/aegisub.desktop" DESTINATION "share/applications")
    install(FILES "packages/desktop/16x16.png" DESTINATION "share/icons/hicolor/16x16/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/22x22.png" DESTINATION "share/icons/hicolor/22x22/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/24x24.png" DESTINATION "share/icons/hicolor/24x24/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/32x32.png" DESTINATION "share/icons/hicolor/32x32/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/48x48.png" DESTINATION "share/icons/hicolor/48x48/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/64x64.png" DESTINATION "share/icons/hicolor/64x64/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/scalable.svg" DESTINATION "share/icons/hicolor/scalable/apps" RENAME aegisub.svg)
endif()
install(TARGETS Aegisub DESTINATION bin)

set(WITH_TEST OFF CACHE BOOL "Enable unit tests")
if(WITH_TEST)
    include("CMakeLists.test.txt")
endif()

message(STATUS "\n"
    "Configure settings\n"
    "  Install prefix:        ${CMAKE_INSTALL_PREFIX}\n"
    "  CFLAGS                 ${CMAKE_C_FLAGS}\n"
    "  CXXFLAGS               ${CMAKE_CXX_FLAGS}\n"
    "\n"
    "Default Settings\n"
    "  Audio Player:          ${DEFAULT_PLAYER_AUDIO}\n"
    "\n"
    "Audio Players\n"
    "  ALSA:                  ${WITH_ALSA}\n"
    "  DirectSound:           ${WITH_DIRECTSOUND}\n"
    "  DirectSound-old:       ${WITH_DIRECTSOUND}\n"
    "  OpenAL:                ${WITH_OPENAL}\n"
    "  OSS:                   ${WITH_OSS}\n"
    "  PortAudio:             ${WITH_PORTAUDIO}\n"
    "  PulseAudio:            ${WITH_LIBPULSE}\n"
    "\n"
    "Misc Packages\n"
    "  AviSynth:              ${WITH_AVISYNTH}\n"
    "  CSRI:                  ${WITH_CSRI}\n"
    "  FFMS2:                 ${WITH_FFMS2}\n"
    "  FFTW3:                 ${WITH_FFTW3}\n"
    "  Hunspell:              ${WITH_HUNSPELL}\n"
    "  uchardet:              ${WITH_UCHARDET}\n"
    "  LuaJIT:                bundled\n"
    "\n"
    "Options\n"
    "  Startup log:           ${WITH_STARTUPLOG}\n"
    "  Update checker:        ${WITH_UPDATE_CHECKER}\n"
    "  Tests:                 ${WITH_TEST}\n"
    "\n"
)
if (WIN32)
    add_definitions("-D_UNICODE" "-DUNICODE")
endif (WIN32)