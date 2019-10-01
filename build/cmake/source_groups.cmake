source_group("ASS" FILES src/ass_dialogue.cpp
    src/ass_entry.cpp
    src/ass_export_filter.cpp
    src/ass_exporter.cpp
    src/ass_file.cpp
    src/ass_override.cpp
    src/ass_style.cpp
    src/ass_style_storage.cpp
    src/ass_karaoke.cpp
    src/ass_attachment.cpp
    src/ass_parser.cpp
    src/subs_controller.cpp)
source_group("AV support" FILES src/avisynth_wrap.cpp
    src/ffmpegsource_common.cpp
    src/MatroskaParser.c
    src/mkv_wrap.cpp)
source_group("Audio" FILES src/fft.cpp
    src/audio_controller.cpp)
source_group("Audio\\Players" FILES src/audio_player.cpp
    src/audio_player_alsa.cpp
    src/audio_player_dsound.cpp
    src/audio_player_dsound2.cpp
    src/audio_player_openal.cpp
    src/audio_player_oss.cpp
    src/audio_player_portaudio.cpp
    src/audio_player_pulse.cpp)
source_group("Audio\\Providers" FILES src/audio_provider_factory.cpp
    src/audio_provider_avs.cpp
    src/audio_provider_ffmpegsource.cpp)
source_group("Audio\\UI" FILES src/audio_box.cpp
    src/audio_colorscheme.cpp
    src/audio_display.cpp
    src/audio_karaoke.cpp
    src/audio_marker.cpp
    src/audio_renderer.cpp
    src/audio_renderer_spectrum.cpp
    src/audio_renderer_waveform.cpp
    src/audio_timing_dialogue.cpp
    src/audio_timing_karaoke.cpp)
source_group("Automation" FILES src/auto4_base.cpp)
source_group("Automation\\Lua" FILES src/auto4_lua.cpp
    src/auto4_lua_assfile.cpp
    src/auto4_lua_dialog.cpp
    src/auto4_lua_progresssink.cpp)
source_group("Automation\\UI" FILES src/dialog_automation.cpp)
source_group("Commands" FILES src/command/app.cpp
    src/command/command.cpp
    src/command/edit.cpp
    src/command/subtitle.cpp
    src/command/time.cpp
    src/command/tool.cpp
    src/command/help.cpp
    src/command/grid.cpp
    src/command/recent.cpp
    src/command/audio.cpp
    src/command/automation.cpp
    src/command/timecode.cpp
    src/command/video.cpp
    src/command/vis_tool.cpp
    src/command/keyframe.cpp)
source_group("Config" FILES src/agi_pre.cpp)
source_group("Controls" FILES src/timeedit_ctrl.cpp
    src/toggle_bitmap.cpp)
source_group("Features\\About box" FILES src/version.cpp
    src/dialog_about.cpp)
source_group("Features\\Align" FILES src/dialog_align.cpp)
source_group("Features\\Attachments" FILES src/dialog_attachments.cpp)
source_group("Features\\Autosave" FILES src/dialog_autosave.cpp)
source_group("Features\\Colour picker" FILES src/colorspace.cpp
    src/dialog_colorpicker.cpp
    src/colour_button.cpp)
source_group("Features\\Conditional select lines" FILES src/dialog_selection.cpp)
source_group("Features\\Export" FILES src/dialog_export.cpp
    src/dialog_export_ebu3264.cpp
    src/export_fixstyle.cpp
    src/export_framerate.cpp)
source_group("Features\\File properties" FILES src/dialog_properties.cpp)
source_group("Features\\Font collector" FILES src/font_file_lister_gdi.cpp
    src/dialog_fonts_collector.cpp
    src/font_file_lister.cpp)
source_group("Features\\Help" FILES src/help_button.cpp
    src/tooltip_manager.cpp)
source_group("Features\\Import" FILES src/dialog_text_import.cpp
    src/charset_detect.cpp)
source_group("Features\\Karaoke copier" FILES src/dialog_kara_timing_copy.cpp)
source_group("Features\\Paste Over" FILES src/dialog_paste_over.cpp)
source_group("Features\\Resolution resampler" FILES src/dialog_resample.cpp
    src/resolution_resampler.cpp
    src/dialog_video_properties.cpp)
source_group("Features\\Search-replace" FILES src/dialog_search_replace.cpp
    src/search_replace_engine.cpp)
source_group("Features\\Shift Times" FILES src/dialog_shift_times.cpp)
source_group("Features\\Spell checker" FILES src/dialog_spellchecker.cpp
    src/spellchecker.cpp
    src/spellchecker_hunspell.cpp)
source_group("Features\\Style editor" FILES src/dialog_style_editor.cpp
    src/dialog_style_manager.cpp
    src/subs_preview.cpp)
source_group("Features\\Styling assistant" FILES src/dialog_styling_assistant.cpp)
source_group("Features\\Thesaurus" FILES src/thesaurus.cpp)
source_group("Features\\Timing post-processor" FILES src/dialog_timing_processor.cpp)
source_group("Features\\Translation Assistant" FILES src/dialog_translation.cpp)
source_group("Features\\Update checker" FILES src/dialog_version_check.cpp)
source_group("Main UI" FILES src/frame_main.cpp
    src/main.cpp
    src/hotkey.cpp
    src/menu.cpp
    src/toolbar.cpp
    src/context.cpp
    src/project.cpp)
source_group("Main UI\\Edit box" FILES src/subs_edit_box.cpp
    src/subs_edit_ctrl.cpp
    src/text_selection_controller.cpp)
source_group("Main UI\\Grid" FILES src/base_grid.cpp
    src/selection_controller.cpp
    src/grid_column.cpp)
source_group("Preferences" FILES src/hotkey_data_view_model.cpp
    src/preferences.cpp
    src/preferences_base.cpp)
source_group("Subtitle formats" FILES src/subtitle_format.cpp
    src/subtitle_format_txt.cpp
    src/subtitle_format_ass.cpp
    src/subtitle_format_ebu3264.cpp
    src/subtitle_format_encore.cpp
    src/subtitle_format_microdvd.cpp
    src/subtitle_format_mkv.cpp
    src/subtitle_format_srt.cpp
    src/subtitle_format_transtation.cpp
    src/subtitle_format_ttxt.cpp
    src/subtitle_format_ssa.cpp)
source_group("Utilities" FILES src/string_codec.cpp
    src/aegisublocale.cpp
    src/compat.cpp
    src/utils.cpp
    src/validators.cpp
    src/text_file_reader.cpp
    src/text_file_writer.cpp
    src/initial_line_state.cpp)
source_group("Utilities\\Logging" FILES src/dialog_log.cpp
    src/crash_writer.cpp
    src/crash_writer_minidump.cpp)
source_group("Utilities\\UI utilities" FILES src/dialog_selected_choices.cpp
    src/dialog_progress.cpp
    src/persist_location.cpp
    src/pen.cpp
    src/image_position_picker.cpp)
source_group("Video" FILES src/video_frame.cpp
    src/video_controller.cpp)
source_group("Video\\Providers" FILES src/video_provider_yuv4mpeg.cpp
    src/video_provider_avs.cpp
    src/video_provider_cache.cpp
    src/video_provider_dummy.cpp
    src/video_provider_ffmpegsource.cpp
    src/video_provider_manager.cpp
    src/async_video_provider.cpp)
source_group("Video\\Subtitle renderers" FILES src/subtitles_provider.cpp
    src/subtitles_provider_libass.cpp
    src/subtitles_provider_csri.cpp)
source_group("Video\\UI" FILES src/video_out_gl.cpp
    src/video_display.cpp
    src/video_box.cpp
    src/dialog_jumpto.cpp
    src/dialog_video_details.cpp
    src/dialog_detached_video.cpp
    src/dialog_dummy_video.cpp
    src/gl_text.cpp
    src/gl_wrap.cpp
    src/video_slider.cpp)
source_group("Video\\Visual tools" FILES src/visual_feature.cpp
    src/visual_tool_vector_clip.cpp
    src/visual_tool.cpp
    src/visual_tool_clip.cpp
    src/visual_tool_cross.cpp
    src/visual_tool_drag.cpp
    src/visual_tool_rotatexy.cpp
    src/visual_tool_rotatez.cpp
    src/visual_tool_scale.cpp
    src/spline.cpp
    src/spline_curve.cpp
    src/vector2d.cpp)
