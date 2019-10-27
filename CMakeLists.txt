cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 17)

project(Aegisub)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build/cmake/modules)

include(build/cmake/resources.cmake)
include(build/cmake/git_version.cmake)

include(libaegisub/libaegisub.cmake) # Not using add_subdirectory to configure dependencies
include_directories("libaegisub/include")
include_directories("src")

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
    src/render_loop.cpp
)

target_link_libraries(Aegisub ${CMAKE_DL_LIBS} libaegisub)
target_compile_definitions(Aegisub PRIVATE CMAKE_BUILD)

include(build/cmake/options.cmake)
include(build/cmake/dependencies_builtin.cmake)
include(${DEPENDENCIES_CMAKE_FILE}) # default: build/cmake/dependencies.cmake
include(build/cmake/dependencies_sources.cmake)

if (MSVC)
    add_compile_options("/utf-8")
    add_compile_definitions("_UNICODE" "UNICODE")

	target_compile_definitions(libaegisub PRIVATE NOMINMAX INITGUID)
	target_compile_definitions(Aegisub PRIVATE NOMINMAX INITGUID)
    target_compile_options(Aegisub PRIVATE "/FI${PROJECT_SOURCE_DIR}/src/agi_pre.h" "-MP")
    target_link_libraries (Aegisub Usp10)
    #target_sources(Aegisub PRIVATE src/res/res.rc src/res/strings.rc src/crash_writer_minidump.cpp)
    target_sources(Aegisub PRIVATE
		src/res/res.rc
		src/res/strings.rc
		src/generated/windows/resource.rc
		src/crash_writer.cpp
		src/dpi_aware.manifest)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Aegisub)
else()
    target_sources(Aegisub PRIVATE src/crash_writer.cpp)
    target_compile_options(Aegisub PRIVATE -include "${PROJECT_SOURCE_DIR}/src/agi_pre.h")
endif()

if(APPLE)
    target_sources(Aegisub PRIVATE src/osx/osx_utils.mm src/osx/retina_helper.mm src/osx/scintilla_ime.mm)
    set(CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000") # luajit
    add_compile_definitions("GL_SILENCE_DEPRECATION")
endif()

include(build/cmake/install.cmake)

if(WITH_TEST)
    include(tests/tests.cmake)
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
    "  fontconfig:            ${WITH_FONTCONFIG}\n"
    "  LuaJIT:                ON\n"
    "\n"
    "Options\n"
    "  Startup log:           ${WITH_STARTUPLOG}\n"
    "  Update checker:        ${WITH_UPDATE_CHECKER}\n"
    "  Tests:                 ${WITH_TEST}\n"
    "\n"
)

include(build/cmake/source_groups.cmake)
