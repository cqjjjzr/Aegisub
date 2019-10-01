[![Build Status](https://travis-ci.org/wangqr/Aegisub.svg?branch=dev)](https://travis-ci.org/wangqr/Aegisub)

# Aegisub

For binaries and general information [see the homepage](http://www.aegisub.org).

The bug tracker can be found at https://github.com/Aegisub/Aegisub/issues .

Support is available on IRC ( irc://irc.rizon.net/aegisub ).

## Building Aegisub

Prerequisites:

1. CMake 3.14 or later (or you can use an older version by editing the first line in CMakeLists.txt, and download the missing `cmake/FindFontconfig.cmake` from [here](https://gitlab.kitware.com/cmake/cmake/blob/master/Modules/FindFontconfig.cmake)),
2. Any compiling toolchain supported by CMake,

### Dependency Resolving

If you want to resoive all the dependencies on your own:

1. All required dependencies, namely `libass`, `Boost`(with ICU support), `OpenGL`, `libicu`, `wxWidgets`, `zlib`, `luabins`, `luajit`. Additionally, `libiconv` is required on non-POSIX systems. `fontconfig` is required on non-Windows systems.
2. Any optional dependencies, namely `ALSA`, `FFMS2`, `FFTW`, `Hunspell`, `OpenAL`, `uchardet`.

If you want to use Conan to automatically get all dependencies:

1. Clone the [CMake script repo using Conan](https://github.com/cqjjjzr/Aegisub-Conan-CMake).
2. Follow the `README.md` in that repository.

### Building

1. If you decided to build from source:
```shell
git clone https://github.com/cqjjjzr/Aegisub.git  # No --recursive is needed
cd Aegisub
git checkout dev-cj # use Charlie's branch
```
2. Make an empty directory to hold build outputs:
```shell
mkdir build-dir
```
3. Build the project using CMake. Use either cmake-gui, or the command line
```shell
cd build-dir
cmake ..
cmake --build .
```

## Updating Moonscript

From within the Moonscript repository, run `bin/moon bin/splat.moon -l moonscript moonscript/ > bin/moonscript.lua`.
Open the newly created `bin/moonscript.lua`, and within it make the following changes:

1. Prepend the final line of the file, `package.preload["moonscript"]()`, with a `return`, producing `return package.preload["moonscript"]()`.
2. Within the function at `package.preload['moonscript.base']`, remove references to `moon_loader`, `insert_loader`, and `remove_loader`. This means removing their declarations, definitions, and entries in the returned table.
3. Within the function at `package.preload['moonscript']`, remove the line `_with_0.insert_loader()`.

The file is now ready for use, to be placed in `automation/include` within the Aegisub repo.

## License

All files in this repository are licensed under various GPL-compatible BSD-style licenses; see LICENCE and the individual source files for more information.
The official Windows build is GPLv2 due to including fftw3.
