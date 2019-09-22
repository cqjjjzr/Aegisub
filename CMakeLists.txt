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
if (UNIX)
    target_sources(libaegisub PRIVATE
        libaegisub/unix/access.cpp
        libaegisub/unix/fs.cpp
        libaegisub/unix/log.cpp
        libaegisub/unix/path.cpp
        libaegisub/unix/util.cpp
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
endif(UNIX)
SET_TARGET_PROPERTIES(libaegisub PROPERTIES PREFIX "")

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
    src/dialog_version_check.cpp
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

if (MSVC)
    target_compile_options(libaegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/libaegisub/lagi_pre.h")
else(MSVC)
    target_compile_options(libaegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/libaegisub/lagi_pre.h")
endif(MSVC)

set_property(
    SOURCE libaegisub/unix/path.cpp
    PROPERTY COMPILE_DEFINITIONS
        P_DATA="${CMAKE_INSTALL_PREFIX}/share/aegisub/"
)

if (MSVC)
    add_definitions("-DNOMINMAX -DINITGUID")
    target_compile_options(Aegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/src/agi_pre.h" "-MP")
    target_link_libraries (Aegisub Usp10)
    #target_sources(Aegisub PRIVATE src/res/res.rc src/res/strings.rc src/crash_writer_minidump.cpp)
    target_sources(Aegisub PRIVATE src/res/res.rc src/res/strings.rc src/generated/windows/resource.rc src/crash_writer.cpp src/dpi_aware.manifest)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Aegisub)
else(MSVC)
    target_sources(Aegisub PRIVATE src/crash_writer.cpp)
    target_compile_options(Aegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/src/agi_pre.h")
endif(MSVC)

if (WIN32)
    target_sources(Aegisub PRIVATE src/font_file_lister_gdi.cpp)
else (WIN32)
    target_sources(Aegisub PRIVATE src/font_file_lister_fontconfig.cpp)
endif (WIN32)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries (Aegisub ${OPENGL_LIBRARIES})

add_definitions("-DWITH_HUNSPELL")
target_sources(Aegisub PRIVATE src/spellchecker_hunspell.cpp)

add_definitions("-DHAVE_ICONV")

find_package(ALSA)
if (ALSA_FOUND)
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries (Aegisub ${ALSA_LIBRARIES})
    add_definitions("-DWITH_ALSA")
    target_sources(Aegisub PRIVATE src/audio_player_alsa.cpp)
endif(ALSA_FOUND)

if (WIN32)
target_compile_definitions(Aegisub PRIVATE "WITH_AVISYNTH")
target_link_libraries(Aegisub Vfw32)
target_sources(Aegisub PRIVATE src/audio_provider_avs.cpp src/avisynth_wrap.cpp src/video_provider_avs.cpp)
endif (WIN32)

target_compile_definitions(Aegisub PRIVATE "WITH_CSRI")
target_sources(Aegisub PRIVATE src/subtitles_provider_csri.cpp)
set_property(SOURCE src/subtitles_provider_csri.cpp PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/vendor/csri/include")

if(MSVC)
    target_link_libraries (Aegisub dsound)
    add_definitions("-DWITH_DIRECTSOUND")
    target_sources(Aegisub PRIVATE src/audio_player_dsound.cpp src/audio_player_dsound2.cpp)
endif(MSVC)

add_definitions("-DWITH_FFMS2")
target_sources(Aegisub PRIVATE src/audio_provider_ffmpegsource.cpp src/ffmpegsource_common.cpp src/video_provider_ffmpegsource.cpp)
add_definitions("-DWITH_FFTW3")

#ifdef WITH_LIBPULSE
#add_definitions("-DWITH_LIBPULSE")
#target_sources(Aegisub PRIVATE src/audio_player_pulse.cpp)

find_package(OpenAL)
if (OPENAL_FOUND)
    include_directories(${OPENAL_INCLUDE_DIR})
    target_link_libraries (Aegisub ${OPENAL_LIBRARY})
    add_definitions("-DWITH_OPENAL")
    target_sources(Aegisub PRIVATE src/audio_player_openal.cpp)
endif(OPENAL_FOUND)

#ifdef WITH_OSS
#ifdef WITH_PORTAUDIO
#ifdef WITH_STARTUPLOG

add_definitions("-DWITH_UCHARDET")

#ifdef WITH_UPDATE_CHECKER

if (WIN32)
    add_definitions("-D_UNICODE" "-DUNICODE")
endif (WIN32)

if (NOT WIN32)
    set(AEGISUB_COMMAND "aegisub" CACHE STRING "The executable name of Aegisub")
    set_target_properties(Aegisub PROPERTIES OUTPUT_NAME aegisub)
    configure_file("packages/desktop/aegisub.desktop.template.in" "aegisub.desktop" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/aegisub.desktop" DESTINATION "share/applications")
    install(FILES "packages/desktop/16x16.png" DESTINATION "share/icons/hicolor/16x16/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/22x22.png" DESTINATION "share/icons/hicolor/22x22/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/24x24.png" DESTINATION "share/icons/hicolor/24x24/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/32x32.png" DESTINATION "share/icons/hicolor/32x32/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/48x48.png" DESTINATION "share/icons/hicolor/48x48/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/64x64.png" DESTINATION "share/icons/hicolor/64x64/apps" RENAME aegisub.png)
    install(FILES "packages/desktop/scalable.svg" DESTINATION "share/icons/hicolor/scalable/apps" RENAME aegisub.svg)
endif (NOT WIN32)
install (TARGETS Aegisub DESTINATION bin)
