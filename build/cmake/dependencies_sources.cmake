target_compile_definitions(libaegisub PRIVATE "HAVE_ICONV")

if(WIN32)
    target_sources(Aegisub PRIVATE src/font_file_lister_gdi.cpp)
endif()
if(WITH_FONTCONFIG)
    target_sources(Aegisub PRIVATE src/font_file_lister_fontconfig.cpp)
endif()

if(WITH_ALSA)
    target_compile_definitions(Aegisub PRIVATE "WITH_ALSA")
    target_sources(Aegisub PRIVATE src/audio_player_alsa.cpp)
endif()

if(WITH_AVISYNTH)
    target_compile_definitions(Aegisub PRIVATE "WITH_AVISYNTH")
    target_sources(Aegisub PRIVATE src/audio_provider_avs.cpp src/avisynth_wrap.cpp src/video_provider_avs.cpp)
endif()

if(WITH_CSRI)
    target_compile_definitions(Aegisub PRIVATE "WITH_CSRI")
    target_sources(Aegisub PRIVATE src/subtitles_provider_csri.cpp)
endif()

if(WITH_DIRECTSOUND)
    target_compile_definitions(Aegisub PRIVATE "WITH_DIRECTSOUND")
    target_sources(Aegisub PRIVATE src/audio_player_dsound.cpp src/audio_player_dsound2.cpp)
endif()

if(WITH_FFMS2)
    target_compile_definitions(Aegisub PRIVATE "WITH_FFMS2")
    target_sources(Aegisub PRIVATE src/audio_provider_ffmpegsource.cpp src/ffmpegsource_common.cpp src/video_provider_ffmpegsource.cpp)
else()
    message(SEND_ERROR
        "No supported video/audio reader interface was enabled.\n"
        "You will not be able to open any video or audio files in Aegisub unless you install a supported video/audio provider.\n"
        "You will however still be able to open \"dummy\" video, ie. a blank, virtual video clip with subtitles overlaid.\n"
        "Currently we only support one video/audio provider on non-Windows systems:\n"
        " - FFMS2\n"
        "   * http://github.com/FFMS/ffms2\n"
    )
endif()

if(WITH_FFTW3)
    target_compile_definitions(Aegisub PRIVATE "WITH_FFTW3")
endif()

if(WITH_HUNSPELL)
    target_compile_definitions(Aegisub PRIVATE "WITH_HUNSPELL")
    target_sources(Aegisub PRIVATE src/spellchecker_hunspell.cpp)
    if(HUNSPELL_HAS_STRING_API)
        target_compile_definitions(Aegisub PRIVATE "HUNSPELL_HAS_STRING_API")
    endif(HUNSPELL_HAS_STRING_API)
endif()

if(WITH_LIBPULSE)
    target_compile_definitions(Aegisub PRIVATE "WITH_LIBPULSE")
    target_sources(Aegisub PRIVATE src/audio_player_pulse.cpp)
endif()

if(WITH_OPENAL)
    target_compile_definitions(Aegisub PRIVATE "WITH_OPENAL")
    target_sources(Aegisub PRIVATE src/audio_player_openal.cpp)
endif()

if(WITH_OSS)
    target_compile_definitions(Aegisub PRIVATE "WITH_OSS")
    target_sources(Aegisub PRIVATE src/audio_player_oss.cpp)
endif()

if(WITH_PORTAUDIO)
    target_compile_definitions(Aegisub PRIVATE "WITH_PORTAUDIO")
    target_sources(Aegisub PRIVATE src/audio_player_portaudio.cpp)
endif()

if(WITH_UCHARDET)
    target_compile_definitions(Aegisub PRIVATE "WITH_UCHARDET")
endif()

if(WIN32)
    set(DEFAULT_PLAYER_AUDIO DirectSound)
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
endif()
