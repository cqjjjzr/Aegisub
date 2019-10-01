if(WITH_CSRI)
    add_subdirectory(dependencies/csri)
    target_include_directories(Aegisub PRIVATE dependencies/csri/include)
    target_link_libraries(Aegisub csri)
endif()

if(WITH_DIRECTSOUND)
    target_link_libraries(Aegisub dsound)
endif()