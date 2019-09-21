cmake_minimum_required(VERSION 3.14)
project(csri)

add_library(csri STATIC
    lib/list.c
    lib/wrap.c
    subhelp/logging.c
)
target_include_directories(csri PRIVATE "include")
if(WIN32)
    target_include_directories(csri PRIVATE "lib/win32")
    target_sources(csri PRIVATE lib/win32/enumerate.c)
else()
    target_include_directories(csri PRIVATE "lib/posix")
    target_sources(csri PRIVATE lib/posix/enumerate.c)
endif()