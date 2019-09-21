from conans import ConanFile, CMake

class AegisubConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "bzip2/1.0.8@conan/stable", \
    "libhunspell/1.7.0@charliejiang/stable", \
    "libass/0.14.0@charliejiang/stable", \
    "uchardet/0.0.6@charliejiang/stable", \
    "fftw/3.3.8@bincrafters/stable", \
    "wxwidgets/3.1.2@bincrafters/stable", \
    "ffms2-core/2.31@charliejiang/stable", \
    "luajit/2.0.5@charliejiang/stable", \
    "luabins/0.3@h4mster/stable", \
    "gtest/1.8.1@bincrafters/stable", \
    "boost/1.71.0@conan/stable", \
    "icu/64.2@bincrafters/stable", \
    "libiconv/1.15@bincrafters/stable"

    generators = "cmake"
    default_options = {}

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")

    def requirements(self):
        self.options["luajit"].lua52_compat = True

        self.options["ffmpeg"].extra_config_flags = "--enable-gpl --enable-runtime-cpudetect --enable-small"
        self.options["ffmpeg"].zlib = True
        self.options["ffmpeg"].freetype = True

        self.options["ffmpeg"].postproc = False
        self.options["ffmpeg"].bzlib = False
        self.options["ffmpeg"].lzma = False
        self.options["ffmpeg"].openjpeg = False
        self.options["ffmpeg"].openh264 = False
        self.options["ffmpeg"].opus = False
        self.options["ffmpeg"].vorbis = False
        self.options["ffmpeg"].zmq = False
        self.options["ffmpeg"].sdl2 = False
        self.options["ffmpeg"].x264 = False
        self.options["ffmpeg"].x265 = False
        self.options["ffmpeg"].vpx = False
        self.options["ffmpeg"].mp3lame = False
        self.options["ffmpeg"].fdk_aac = False
        self.options["ffmpeg"].webp = False
        self.options["ffmpeg"].mp3lame = False
        self.options["ffmpeg"].openssl = False

        if self.settings.os == "Windows":
            self.options["ffmpeg"].qsv = False
        if self.settings.os == "Macos":
            self.options["ffmpeg"].appkit = False
            self.options["ffmpeg"].avfoundation = False
            self.options["ffmpeg"].coreimage = False
            self.options["ffmpeg"].audiotoolbox = False
            self.options["ffmpeg"].videotoolbox = False
            self.options["ffmpeg"].securetransport = False
        if self.settings.os == "Linux":
            self.options["ffmpeg"].vaapi = False
            self.options["ffmpeg"].vdpau = False
            self.options["ffmpeg"].xcb = False
            self.options["ffmpeg"].alsa = False
            self.options["ffmpeg"].vaapi = False
            self.options["ffmpeg"].pulse = False

        self.options["wxwidgets"].stc = True
        self.options["wxwidgets"].richtext = True
        self.options["wxwidgets"].sockets = True # this sucks

        self.options["wxwidgets"].jpeg = "off"
        self.options["wxwidgets"].tiff = "off"
        self.options["wxwidgets"].secretstore = False
        self.options["wxwidgets"].aui = False
        self.options["wxwidgets"].html = True
        self.options["wxwidgets"].mediactrl = False
        self.options["wxwidgets"].propgrid = False
        self.options["wxwidgets"].debugreport = False
        self.options["wxwidgets"].ribbon = False
        self.options["wxwidgets"].webview = False
        self.options["wxwidgets"].xml = False
        self.options["wxwidgets"].xrc = False
        if self.settings.os == "Linux":
            self.options["wxwidgets"].cairo = False

        boost_using_list = ['chrono', 'filesystem', 'locale', 'regex', 'system', 'thread']
        boost_lib_list = ['math', 'wave', 'container', 'contract', 'exception', 'graph', 'iostreams', 'locale', 'log',
            'program_options', 'random', 'regex', 'mpi', 'serialization',
            'coroutine', 'fiber', 'context', 'timer', 'thread', 'chrono', 'date_time',
            'atomic', 'filesystem', 'system', 'graph_parallel', 'python',
            'stacktrace', 'test', 'type_erasure']
        for lib in [x for x in boost_lib_list if x not in boost_using_list]:
            self.options["boost"].add("without_" + lib + "=True")
